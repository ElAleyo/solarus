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
#ifndef SOLARUS_NPC_H
#define SOLARUS_NPC_H

#include "Common.h"
#include "entities/Detector.h"
#include "KeysEffect.h"

/**
 * \brief A non-playing character or a more general interactive entity.
 *
 * The entity triggers a message or an event
 * when the player presses the action key while facing it.
 *
 * This class can be used for usual living NPCs who the hero can talk to,
 * but also for more general non-living, interactive objects that show a
 * dialog or trigger a script, like signs or torches.
 * Such customizable interactive objects are called generalized NPCs.
 *
 * A usual NPC has some specific collision and displaying properties.
 * It must have a sprite with four directions, an animation "stopped",
 * and an animation "walking" if the script starts a movement.
 * This class handles automatically the sprite's direction and animations
 * depending on the movement and the interactions with the hero.
 *
 * On the contrary, a generalized NPC is just assumed to be a solid
 * interactive entity, and the map script has to handle explicitly its
 * animations (if any).
 */
class NPC: public Detector {

  public:

    /**
     * \brief The different kinds of interactions.
     */
    enum Subtype {
      GENERALIZED_NPC,    /**< a custom interactive entity (not necessarily a person) */
      USUAL_NPC           /**< a usual NPC who the player can talk to */
    };

    /**
     * \brief Type of action done when an interaction occurs.
     */
    enum Behavior {
       BEHAVIOR_DIALOG,                 /**< start a dialog */
       BEHAVIOR_MAP_SCRIPT,             /**< notify the NPC in Lua */
       BEHAVIOR_ITEM_SCRIPT             /**< notify an equipment item in Lua */
    };

  private:

    Subtype subtype;                    /**< subtpype of NPC */
    Behavior behavior;                  /**< type of action done when the player interacts with this entity */
    std::string dialog_to_show;         /**< dialog to show when an interaction occurs, or an empty string */
    std::string item_name;              /**< name of an equipment item to notify when an interaction occurs */

    void initialize_sprite(const std::string& sprite_name, int initial_direction);
    void call_script_hero_interaction();

  public:

    NPC(Game& game, const std::string& name, Layer layer, int x, int y,
        Subtype subtype, const std::string& sprite_name,
        int initial_direction, const std::string& behavior_string);
    ~NPC();

    EntityType get_type() const;

    bool is_solid();

    bool is_drawn_in_y_order();
    bool is_obstacle_for(MapEntity& other);
    bool is_hero_obstacle(Hero& hero);
    bool is_npc_obstacle(NPC& npc);
    bool is_enemy_obstacle(Enemy& enemy);
    bool is_sword_ignored();

    void notify_collision(MapEntity& entity_overlapping, CollisionMode collision_mode);
    void notify_action_command_pressed();
    bool interaction_with_item(EquipmentItem& item);
    void notify_position_changed();
    void notify_movement_finished();
    bool can_be_lifted();

    virtual const std::string& get_lua_type_name() const;
};

#endif

