#pragma once

const float LOGIC_WIDTH = 1600.f;
const float LOGIC_HEIGHT = 900.f;
const float Z_NEAR = -20.f;
const float Z_FAR = 20.f;
const float GRID_SQUARE_OFFSET = 75.f / 4.f;
const float GAME_SPACE_Y = LOGIC_HEIGHT * 3.f / 4.f;
const float TILE_LENGTH = (GAME_SPACE_Y - 4.f * GRID_SQUARE_OFFSET) / 4.f;
const float TILE_GRID_START_X = GRID_SQUARE_OFFSET * 1.5f + TILE_LENGTH / 2;

const float SHOP_TILE_LENGTH = TILE_LENGTH;
const float SHOP_TILE_OFFSET = GRID_SQUARE_OFFSET;
const float SHOP_OFFSET = GRID_SQUARE_OFFSET;

const float SHOP_ITEM_SCALE = SHOP_TILE_LENGTH * 0.8f;
const float SHOP_TILES_COUNT = 4;

const float TURRET_SCALE = TILE_LENGTH * 0.8f;
const float ENEMY_SCALE = TURRET_SCALE * 0.8f;
const float PROJECTILE_SCALE = TURRET_SCALE * 0.3f;

const float TURRET_GUN_OFFSET_Y = (0.333333f - 0.0313725f) / 2.f * TURRET_SCALE;
const float TURRET_GUN_OFFSET_X = 0.498039f / 2.f * TURRET_SCALE;

const float CHAR_OFFSET = LOGIC_WIDTH / 200.f;
const float SPACE_OFFSET = .7f;

const float HEART_SCALE = 100.f;

const float ENEMY_DESTROYED_SCORE = 50.f;

const float STAR_SCALE = 60.f;