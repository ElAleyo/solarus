/*
 * Copyright (C) 2006-2013 Christopho, Solarus - http://www.solarus-games.org
 *
 * Solarus is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Solarus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "lua/LuaContext.h"
#include "lowlevel/FileTools.h"
#include "StringResource.h"
#include "DialogResource.h"
#include "QuestResourceList.h"
#include <vector>

const std::string LuaContext::language_module_name = "sol.language";

/**
 * \brief Pushes a dialog onto the stack.
 * \param l A Lua context.
 * \param dialog A dialog.
 */
void LuaContext::push_dialog(lua_State* l, const Dialog& dialog) {

  lua_newtable(l);

  // Dialog id.
  push_string(l, dialog.get_id());
  lua_setfield(l, -2, "id");

  // Text.
  push_string(l, dialog.get_text());
  lua_setfield(l, -2, "text");

  // User properties.
  const std::map<std::string, std::string>& properties =
      dialog.get_properties();
  std::map<std::string, std::string>::const_iterator it;
  for (it = properties.begin(); it != properties.end(); ++it) {
    push_string(l, it->second);
    lua_setfield(l, -2, it->first.c_str());
  }
}

/**
 * \brief Initializes the language features provided to Lua.
 */
void LuaContext::register_language_module() {

  static const luaL_Reg functions[] = {
      { "get_language", language_api_get_language },
      { "set_language", language_api_set_language },
      { "get_language_name", language_api_get_language_name },
      { "get_languages", language_api_get_languages },
      { "get_string", language_api_get_string },
      { "get_dialog", language_api_get_dialog },
      { NULL, NULL }
  };
  register_functions(language_module_name, functions);
}

/**
 * \brief Implementation of sol.language.get_language().
 * \param l the Lua context that is calling this function
 * \return number of values to return to Lua
 */
int LuaContext::language_api_get_language(lua_State* l) {

  const std::string& language = FileTools::get_language();

  if (language.empty()) {  // Return nil if no language is set.
    lua_pushnil(l);
  }
  else  {  // Return the language code.
    push_string(l, language);
  }
  return 1;
}

/**
 * \brief Implementation of sol.language.set_language().
 * \param l the Lua context that is calling this function
 * \return number of values to return to Lua
 */
int LuaContext::language_api_set_language(lua_State* l) {

  const std::string& language_code = luaL_checkstring(l, 1);

  if (!FileTools::has_language(language_code)) {
    arg_error(l, 1, std::string("No such language: '") + language_code + "'");
  }
  FileTools::set_language(language_code);

  return 0;
}

/**
 * \brief Implementation of sol.language.get_language_name().
 * \param l the Lua context that is calling this function
 * \return number of values to return to Lua
 */
int LuaContext::language_api_get_language_name(lua_State* l) {

  std::string language_code;
  if (lua_gettop(l) >= 1) {
    language_code = luaL_checkstring(l, 1);
    if (!FileTools::has_language(language_code)) {
      arg_error(l, 1, std::string("No such language: '") + language_code + "'");
    }
  }
  else {
    language_code = FileTools::get_language();
    if (language_code.empty()) {
      error(l, "No language is set");
    }
  }

  const std::string& name = FileTools::get_language_name(language_code);
  push_string(l, name);

  return 1;
}

/**
 * \brief Implementation of sol.language.get_languages().
 * \param l The Lua context that is calling this function.
 * \return Number of values to return to Lua.
 */
int LuaContext::language_api_get_languages(lua_State* l) {

  const std::vector<QuestResourceList::Element>& languages =
    QuestResourceList::get_elements(QuestResourceList::RESOURCE_LANGUAGE);

  lua_newtable(l);
  int i = 1;
  std::vector<QuestResourceList::Element>::const_iterator it;
  for (it = languages.begin(); it != languages.end(); ++it) {
    const std::string& language_code = it->first;
    push_string(l, language_code);
    lua_rawseti(l, -2, i);
    ++i;
  }

  return 1;
}

/**
 * \brief Implementation of sol.language.get_string().
 * \param l the Lua context that is calling this function
 * \return number of values to return to Lua
 */
int LuaContext::language_api_get_string(lua_State* l) {

  const std::string& key = luaL_checkstring(l, 1);

  if (!StringResource::exists(key)) {
    lua_pushnil(l);
  }
  else {
    push_string(l, StringResource::get_string(key));
  }
  return 1;
}

/**
 * \brief Implementation of sol.language.get_dialog().
 * \param l the Lua context that is calling this function
 * \return number of values to return to Lua
 */
int LuaContext::language_api_get_dialog(lua_State* l) {

  const std::string& dialog_id = luaL_checkstring(l, 1);

  if (!DialogResource::exists(dialog_id)) {
    lua_pushnil(l);
  }
  else {
    push_dialog(l, DialogResource::get_dialog(dialog_id));
  }
  return 1;
}

