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
#ifndef SOLARUS_DIALOG_BOX_H
#define SOLARUS_DIALOG_BOX_H

#include "Common.h"
#include "Dialog.h"
#include "GameCommands.h"
#include "lowlevel/Rectangle.h"
#include <list>

/**
 * \brief Manages the dialog box where dialogs can be displayed.
 *
 * A very basic built-in dialog box is used by default, but the quest
 * script can define its own one instead.
 * This class handles both cases.
 */
class DialogBox {

  public:

    DialogBox(Game& game);
    ~DialogBox();

    Game& get_game();
    bool is_enabled();

    void open(const std::string& dialog_id, int info_ref, int callback_ref);
    void close(int status_ref);
    bool notify_command_pressed(GameCommands::Command command);
    const std::string& get_dialog_id();

    void draw(Surface& dst_surface);

  private:

    bool has_more_lines();
    void show_more_lines();

    Game& game;                                     /**< The game this dialog box belongs to. */
    std::string dialog_id;                          /**< Id of the current dialog or an empty string. */
    Dialog dialog;                                  /**< Current dialog. */
    int callback_ref;                               /**< Lua ref of a function to call when the dialog finishes. */

    // Fields only used by the built-in dialog box.
    bool built_in;                                  /**< Whether we are using the built-in dialog box. */
    static const int nb_visible_lines = 3;          /**< Maximum number of visible lines. */
    std::list<std::string> remaining_lines;         /**< Text of each line still to be displayed. */
    TextSurface* line_surfaces[nb_visible_lines];   /**< Text surface of each visible line. */
    Rectangle text_position;                        /**< Destination position of the first line. */
    bool is_question;                               /**< Whether the dialog is a question with two possible answers. */
    bool selected_first_answer;                     /**< If there is a question: whether the first or second answer is selected. */
};

#endif
