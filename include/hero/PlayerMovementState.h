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
#ifndef SOLARUS_HERO_PLAYER_MOVEMENT_STATE_H
#define SOLARUS_HERO_PLAYER_MOVEMENT_STATE_H

#include "Common.h"
#include "hero/State.h"

/**
 * Abstract class for all hero states such that the movement is controlled by the player.
 * This means that the hero can move in the eight directions and the movement
 * is an instance of PlayerMovement.
 * These states include FreeState (the normal walk), SwordLoadingState (the sword is
 * loading), CarryingState (the hero is carrying something) and SwimmingState.
 */
class Hero::PlayerMovementState: public Hero::State {

  public:

    virtual ~PlayerMovementState();
    virtual void start(State* previous_state);
    virtual void stop(State* next_state);
    void set_map(Map& map);

    virtual void update();
    virtual void set_suspended(bool suspended);
    virtual void set_animation_stopped();
    virtual void set_animation_walking();
    bool can_control_movement();
    int get_wanted_movement_direction8();
    void notify_walking_speed_changed();
    void notify_movement_changed();
    void notify_position_changed();
    void notify_layer_changed();
    bool can_be_hurt(Enemy* attacker);
    bool can_pick_treasure(EquipmentItem& item);
    bool can_take_jumper();
    void notify_jumper_activated(Jumper& jumper);

  protected:

    PlayerMovementState(Hero& hero, const std::string& state_name);

    PlayerMovement* get_player_movement();

  private:

    void cancel_jumper();

    Jumper* current_jumper;            /**< The jumper about to be triggered or NULL */
    uint32_t jumper_start_date;        /**< Date to trigger the jumper
                                        * (because a small delay is necessary) */
};

#endif

