/* Copyright (c) 2019  Roger Ferrer <rofirrim@gmail.com>
 *
 * This file is part of libadwaitamm.
 *
 * libadwaitamm is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 2.1 of the License,
 * or (at your option) any later version.
 *
 * libadwaitamm is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// We always need to generate the .defs for all types because the code
// using deprecated API is generated unconditionally and only disabled
// at compile time.
#undef LIBADWAITA_DISABLE_DEPRECATED

#include <glibmm_generate_extra_defs/generate_extra_defs.h>
#include <libadwaita-1/adwaita.h>
#include <iostream>

int main(int, char**)
{
  adw_init ();

  std::cout << get_defs(ADW_TYPE_WINDOW);
  std::cout << get_defs(ADW_TYPE_ABOUT_WINDOW);
  std::cout << get_defs(ADW_TYPE_PREFERENCES_ROW);
  std::cout << get_defs(ADW_TYPE_ACTION_ROW);
  std::cout << get_defs(ADW_TYPE_WINDOW_TITLE);
  std::cout << get_defs(ADW_TYPE_ANIMATION);
  std::cout << get_defs(ADW_TYPE_APPLICATION);
  std::cout << get_defs(ADW_TYPE_STYLE_MANAGER);
  std::cout << get_defs(ADW_TYPE_APPLICATION_WINDOW);
  std::cout << get_defs(ADW_TYPE_AVATAR);
  std::cout << get_defs(ADW_TYPE_BIN);
  std::cout << get_defs(ADW_TYPE_BUTTON_CONTENT);

  return 0;
}
