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
#ifndef SOLARUS_TARGET_MOVEMENT_H
#define SOLARUS_TARGET_MOVEMENT_H

#include "Common.h"
#include "movements/StraightMovement.h"

/**
 * \brief Movement of an object that goes to a target point.
 *
 * The target point may be a fixed point or a moving entity.
 */
class TargetMovement: public StraightMovement {

  public:

    TargetMovement(
        MapEntity* target_entity,
        int x,
        int y,
        int moving_speed,
        bool ignore_obstacles);
    ~TargetMovement();

    void set_target(MapEntity* target_entity, int x, int y);

    int get_moving_speed();
    void set_moving_speed(int moving_speed);

    virtual void notify_object_controlled();
    bool is_finished();
    void update();

    virtual const std::string& get_lua_type_name() const;

  private:

    void recompute_movement();

    int target_x;                      /**< X coordinate of the point or entity to track. */
    int target_y;                      /**< Y coordinate of the point or entity to track. */
    MapEntity* target_entity;          /**< The entity to track (NULL if only
                                        * a point is targeted) */
    int entity_offset_x;               /**< X value to add to the entity's coordinates. */
    int entity_offset_y;               /**< Y value to add to the entity's coordinates. */

    int sign_x;                        /**< Sign of the x movement (1: right, -1: left) */
    int sign_y;                        /**< Sign of the y movement (1: down, -1: up) */
    int moving_speed;                  /**< Speed when moving */

    static const uint32_t recomputation_delay; /**< Delay between two recomputations. */
    uint32_t next_recomputation_date;  /**< Date when the movement is recalculated. */
    bool finished;				       /**< \c true if the target is reached. */

};

#endif

