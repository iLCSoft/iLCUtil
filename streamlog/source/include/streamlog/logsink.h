// -*- mode: c++;
#ifndef streamlog_logsink_h
#define streamlog_logsink_h

// -- std headers
#include <map>
#include <mutex>
#include <iostream>
#include <fstream>
#include <stdexcept>

// -- streamlog headers
#include <streamlog/definitions.h>
#include <streamlog/formatter.h>
#include <streamlog/logmessage.h>

namespace streamlog {

  class logsink ;
  typedef std::shared_ptr<logsink> logsink_ptr ;

  class logsink {
  public:
    typedef std::unique_ptr<formatter> formatter_ptr ;

    logsink() = default ;
    virtual ~logsink() = default ;

  public:
    /**
     *  @brief  log the message
     *
     *  @param  msg the message to log
     */
    virtual void log( const logcontext &ctx, const std::string &msg ) = 0 ;

    /**
     *  @brief  Set the log message formatter to use
     *
     *  @param  formatter the formatter to use
     */
    virtual void setFormatter( formatter_ptr formatter ) = 0 ;
    
    virtual std::string prefix( const logcontext &ctx ) = 0 ;

  protected:
    /// The log message formatter
    formatter_ptr          _formatter {streamlog::make_unique<standard_formatter>()} ;
  };
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  template <typename mutex_type>
  class base_sink : public logsink {
  public:
    base_sink() = default ;
    virtual ~base_sink() = default ;

    /**
     *  @brief  log the message
     *
     *  @param  msg the message to log
     */
    void log( const logcontext &ctx, const std::string &msg ) ;

    std::string prefix( const logcontext &ctx ) ;

    /**
     *  @brief  Set the log message formatter to use
     *
     *  @param  formatter the formatter to use
     */
    void setFormatter( formatter_ptr formatter ) ;

  protected:
    /**
     *  @brief  Perform the actual logging operation
     *
     *  @param  msg the message to log
     */
    virtual void doLog( const logcontext &ctx, const std::string &msg ) = 0 ;

  private:
    /// The logging mutex
    mutex_type             _mutex {} ;
  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline void base_sink<mutex_type>::log( const logcontext &ctx, const std::string &msg ) {
    std::lock_guard<mutex_type> lock( _mutex ) ;
    doLog( msg ) ;
  }
  
  //--------------------------------------------------------------------------
  
  template <typename mutex_type>
  inline std::string base_sink<mutex_type>::prefix( const logcontext &ctx ) {
    std::lock_guard<mutex_type> lock( _mutex ) ;
    return _formatter->prefix( ctx ) ;
  }

  //--------------------------------------------------------------------------

  template <typename mutex_type>
  inline void base_sink<mutex_type>::setFormatter( formatter_ptr formatter ) {
    std::lock_guard<mutex_type> lock( _mutex ) ;
    _formatter = std::move( formatter ) ;
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  console_sink class
   *  Standard std::cout output
   */
  template <typename mutex_type>
  class console_sink : public base_sink<mutex_type> {
  public:
    console_sink() = default ;

  private:
    void doLog( const logcontext &/*ctx*/, const std::string &msg ) {
      std::cout << msg ;
    }
  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  colored_console_sink class
   *  Standard std::cout output with fancy colors
   */
  template <typename mutex_type>
  class colored_console_sink : public base_sink<mutex_type> {
  public:
    colored_console_sink() = default ;

  private:
    void doLog( const logcontext &ctx, const std::string &msg ) {
      std::cout 
        << color( ctx._level ) 
        << msg
        << color::reset ;
    }

    const color_helper &color( unsigned int level ) const {
      if ( level >= streamlog::error_base_level ) {
        return color::bold_red ;
      }
      if ( level >= streamlog::warning_base_level ) {
        return color::bold_yellow ;
      }
      if ( level >= streamlog::message_base_level ) {
        return color::bold_green ;
      }
      if ( level >= streamlog::debug_base_level ) {
        return color::bold_blue ;
      }
      return color::default_color ;
    }

  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  simple_file_sink class
   *  Standard std::cout output
   */
  template <typename mutex_type>
  class simple_file_sink : public base_sink<mutex_type> {
  public:
    /**
     *  @brief  Constructor
     *
     *  @param  fname the log file name
     *  @param  mode the opening mode
     */
    simple_file_sink( std::string &fname, std::ios_base::openmode mode = std::ios_base::out ) {
      _fstream.open( fname, mode ) ;
      if ( not _fstream.is_open() ) {
        throw std::runtime_error( "simple_file_sink: couldn't open log file '" + fname + "'!" ) ;
      }
    }

    /**
     *  @brief  Destructor. Close the file stream
     */
    ~simple_file_sink() {
      _fstream.close() ;
    }

  private:
    void doLog( const logcontext &ctx, const std::string &msg ) {
      _fstream << msg ;
    }

  private:
    /// The log file stream
    std::ofstream           _fstream {} ;
  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  thread_file_sink class
   *  Opens a log file per thread and log respective message in
   *  the different log file depending on the log message thread id
   */
  class thread_file_sink : public base_sink<std::mutex> {
  public:
    using thread_id = logcontext::thread_id ;
    using stream_map = std::map<thread_id, std::ofstream> ;

  public:
    /**
     *  @brief  Constructor
     *
     *  @param  basename the base log file name
     *  @param  mode the opening mode
     */
    thread_file_sink(
      const std::string &basename,
      const std::string &extension,
      std::ios_base::openmode mode = std::ios_base::out ) :
      _basename(basename),
      _extension(extension),
      _openmode(mode) {
      /* nop */
    }

    /**
     *  @brief  Destructor. Close the file streams
     */
    ~thread_file_sink() {
      for ( auto &s : _fstreams ) {
        s.second.close() ;
      }
    }

  private:
    void doLog( const logcontext &ctx, const std::string &msg ) {
      stream( ctx._threadId ) << msg ;
    }

    std::ofstream &stream( thread_id id ) {
      auto iter = _fstreams.find( id ) ;
      if ( _fstreams.end() == iter ) {
        iter = _fstreams.insert( stream_map::value_type( id, open(id) ) ).first ;
      }
      return iter->second ;
    }

    std::ofstream open( thread_id id ) const {
      std::ofstream fs ;
      std::stringstream ss ;
      ss << _basename << "_" << id << _extension ;
      fs.open( ss.str() , _openmode ) ;
      if ( not fs.is_open() ) {
        throw std::runtime_error( "thread_file_sink: couldn't open log file '" + ss.str() + "'!" ) ;
      }
      return std::move( fs ) ;
    }

  private:
    /// The log file stream map (map with thread id)
    stream_map              _fstreams {} ;
    /// The log file base name
    std::string             _basename {} ;
    /// The log file extension
    std::string             _extension {} ;
    /// The log file open mode
    std::ios_base::openmode _openmode {} ;
  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  using simple_file_sink_st = simple_file_sink<streamlog::nullmutex> ;
  using simple_file_sink_mt = simple_file_sink<std::mutex> ;
  using console_sink_st = console_sink<streamlog::nullmutex> ;
  using console_sink_mt = console_sink<std::mutex> ;
  using colored_console_sink_st = colored_console_sink<streamlog::nullmutex> ;
  using colored_console_sink_mt = colored_console_sink<std::mutex> ;

}

#endif
