#include "streamlog/definitions.h"

namespace streamlog {

  const color_helper color::default_color   {color_code::fg_default, false};
  const color_helper color::reset           {};
  const color_helper color::normal_black    {color_code::fg_black, false} ;
  const color_helper color::normal_red      {color_code::fg_red, false} ;
  const color_helper color::normal_green    {color_code::fg_green, false} ;
  const color_helper color::normal_yellow   {color_code::fg_yellow, false} ;
  const color_helper color::normal_blue     {color_code::fg_blue, false} ;
  const color_helper color::normal_magenta  {color_code::fg_magenta, false} ;
  const color_helper color::normal_cyan     {color_code::fg_cyan, false} ;
  const color_helper color::normal_white    {color_code::fg_white, false} ;
  const color_helper color::bold_black      {color_code::fg_black, true} ;
  const color_helper color::bold_red        {color_code::fg_red, true} ;
  const color_helper color::bold_green      {color_code::fg_green, true} ;
  const color_helper color::bold_yellow     {color_code::fg_yellow, true} ;
  const color_helper color::bold_blue       {color_code::fg_blue, true} ;
  const color_helper color::bold_magenta    {color_code::fg_magenta, true} ;
  const color_helper color::bold_cyan       {color_code::fg_cyan, true} ;
  const color_helper color::bold_white      {color_code::fg_white, true} ;

}
