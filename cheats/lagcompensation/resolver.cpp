// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*#include "animation_system.h"
#include "..\ragebot\aim.h"

void resolver::initialize(player_t* e, adjust_data* record, const float& goal_feet_yaw, const float& pitch)
{
	player = e;
	player_record = record;

	original_goal_feet_yaw = math::normalize_yaw(goal_feet_yaw);
	original_pitch = math::normalize_pitch(pitch);
}

float resolve_value = -75.f;
float resolve_value1 = 75.f;
float resolve_yaw = 65;

void resolver::reset()
{
	player = nullptr;
	player_record = nullptr;

	side = false;
	fake = false;

	was_first_bruteforce = false;
	was_second_bruteforce = false;

	original_goal_feet_yaw = 0.0f;
	original_pitch = 0.0f;
}

void resolver::resolve_yaw()
{
	player_info_t player_info;

	if (!m_engine()->GetPlayerInfo(player->EntIndex(), &player_info))
		return;

		switch (g_ctx.globals.missed_shots[player->EntIndex()] % 7)
		{
		case 0:
			player_record->type = ORIGINAL;
			player_record->side = RESOLVER_ORIGINAL;
			break;
		case 1:
			player_record->type = ZARO;
			player_record->side = RESOLVER_ZERO;
			return;
		case 2:
			player_record->type = FIRST2;
			player_record->side = RESOLVER_FIRST;
			return;
		case 3:
			player_record->type = SECOND2;
			player_record->side = RESOLVER_SECOND;
			return;
		case 4:
			player_record->type = THIRD;
			player_record->side = RESOLVER_THIRD;
			return;
		case 5:
			player_record->type = LOW_FIRST;
			player_record->side = RESOLVER_LOW_FIRST;
			return;
		case 6:
			player_record->type = LOW_SECOND;
			player_record->side = RESOLVER_LOW_SECOND;
			return;
		}
}


float resolver::resolve_pitch()
{
	return original_pitch;
}

*/


#include "animation_system.h"
#include "..\ragebot\aim.h"

void resolver::initialize(player_t* e, adjust_data* record, const float& goal_feet_yaw, const float& pitch)
{
    player = e;
    player_record = record;

    original_goal_feet_yaw = math::normalize_yaw(goal_feet_yaw);
    original_pitch = math::normalize_pitch(pitch);
}

void resolver::reset()
{
    player = nullptr;
    player_record = nullptr;

    side = false;
    fake = false;

    was_first_bruteforce = false;
    was_second_bruteforce = false;

    original_goal_feet_yaw = 0.0f;
    original_pitch = 0.0f;
}

void resolver::resolve_yaw()
{
    float no_side = FLT_MAX;
    float resolve_value = -75.f;
    float resolve_value1 = 75.f;
    float resolve_yaw = 65;
    auto animstate = player->get_animation_state();

    AnimationLayer layers[15];
    AnimationLayer moveLayers[3][15];
    memcpy(moveLayers, player->get_animlayers(), sizeof(AnimationLayer) * 15);

    auto speed = player->m_vecVelocity().Length2D();
    int resolving_way;

    if (speed < 0.1f)
    {
        auto delta = math::AngleDiff(player->m_angEyeAngles().y, no_side);

        if (layers[3].m_flWeight == 0.0f && layers[3].m_flCycle == 0.0f)
        {
            resolving_way = math::clamp((2 * (delta <= 0.f) - 1), -1, 1);
        }
    }
    else if (!int(layers[12].m_flWeight * 1000.f))
    {
        if (int(layers[6].m_flWeight * 1000.f) == int(layers[6].m_flWeight * 1000.f))
        {
            float delta1 = abs(layers[6].m_flPlaybackRate - moveLayers[0][6].m_flPlaybackRate);

            float delta2 = abs(layers[6].m_flPlaybackRate - moveLayers[1][6].m_flPlaybackRate);

            float delta3 = abs(layers[6].m_flPlaybackRate - moveLayers[2][6].m_flPlaybackRate);

            if (delta1 < delta3 || delta2 <= delta3 || (int)(float)(delta3 * 1000.0f))
            {
                if (delta1 >= delta2 && delta3 > delta2 && !(int)(float)(delta2 * 1000.0f))
                {
                    resolving_way = 1;
                }
            }
            else
            {
                resolving_way = -1;
            }
        }
    }

    if (g_ctx.globals.missed_shots[player->EntIndex()] > 0)
    {
        switch (g_ctx.globals.missed_shots[player->EntIndex()] % 2)
        {
        case 0:
            animstate->m_flGoalFeetYaw = math::normalize_yaw(player->m_angEyeAngles().y + 58.f);
            break;
        case 1:
            animstate->m_flGoalFeetYaw = math::normalize_yaw(player->m_angEyeAngles().y - 58.f);
            break;
        }
    }
}

float resolver::resolve_pitch()
{
    return original_pitch;
}




/* TEST */
/*
#include "animation_system.h"
#include "..\ragebot\aim.h"

void resolver::initialize(player_t* e, adjust_data* record, const float& goal_feet_yaw, const float& pitch)
{
    player = e;
    player_record = record;

    original_goal_feet_yaw = math::normalize_yaw(goal_feet_yaw);
    original_pitch = math::normalize_pitch(pitch);
}

void resolver::reset()
{
    player = nullptr;
    player_record = nullptr;

    side = false;
    fake = false;

    was_first_bruteforce = false;
    was_second_bruteforce = false;

    original_goal_feet_yaw = 0.0f;
    original_pitch = 0.0f;
}

void resolver::resolve_yaw()
{
   // float no_side = FLT_MAX; 

    auto animstate = player->get_animation_state();
    auto speed = player->m_vecVelocity().Length2D();
    //int resolving_way; 

    if (speed <= 1.1f)
    {
        auto delta = math::AngleDiff(player->m_angEyeAngles().y, animstate->flFootYaw);

        if (abs(delta) > 35)
        {
            resolving_way = delta > 0 ? -1 : 1;
        }
        else
        {
          resolving_way =
(math::normalize_yaw(ApproachAngle(
                player_record->lby,
                original_goal_feet_yaw,
                animstate->m_flUpdateTimeDelta * 100.0f)) - original_goal_feet_yaw)  > 0 ? 1 : -1;
        }
    }
    else if (!int(layers[12].m_flWeight * 1000.f))..
    {
       // if (int(layers[6].m_flWeight * 1000.f) == int(layers[6].m_flWeight * 1000.f)) 
        if (int(layers[6].m_flWeight * 1000.f) == int(prev_server_layers[6].m_flWeight * 1000.f))
        {
            float delta1 = fabsf(layers[6].m_flPlaybackRate - moveLayers[0][6].m_flPlaybackRate); // middle
            float delta2 = fabsf(layers[6].m_flPlaybackRate - moveLayers[1][6].m_flPlaybackRate); // left
            float delta3 = fabsf(layers[6].m_flPlaybackRate - moveLayers[2][6].m_flPlaybackRate); // right

            if (delta1 < delta2 || delta3 <= delta2 || (int)(float)(delta2 * 1000.0f))
            {
                if (delta1 >= delta3 && delta2 > delta3 && !(int)(float)(delta3 * 1000.0f))
                {
                    resolving_way = 1; // Right
                }
            }
            else
            {
                resolving_way = -1; // Left
            }

           last_delta1 = delta1;
           last_delta2 = delta2;
           last_delta3 = delta3;
        }
       else
       {
           if (last_delta2 > 0 || last_delta3 > 0)
           {
            float delta2 = fabsf(layers[6].m_flPlaybackRate - moveLayers[1][6].m_flPlaybackRate); // left
            float delta3 = fabsf(layers[6].m_flPlaybackRate - moveLayers[2][6].m_flPlaybackRate); // right

             if (delta2 < last_delta2 && delta3 >= last_delta3)
             {
                    resolving_way = -1; // Left
             }
             else if (delta3 < delta2 || last_delta3 < last_delta2)
             {
                resolving_way = 1; // Right
             }
           }
       }
    }

        switch (g_ctx.globals.missed_shots[player->EntIndex()] % 5)
        {
        case 0:
            animstate->m_flGoalFeetYaw = math::normalize_yaw(player->m_angEyeAngles().y + 60.f * resolving_way );
            break;
        case 1:
            animstate->m_flGoalFeetYaw = math::normalize_yaw(player->m_angEyeAngles().y - 60.f * resolving_way );
            break;
        case 2:
            animstate->m_flGoalFeetYaw = math::normalize_yaw(player->m_angEyeAngles().y + 45 * resolving_way );
            break;
        case 3:
            animstate->m_flGoalFeetYaw = math::normalize_yaw(player->m_angEyeAngles().y - 45.f * resolving_way );
            break;
        case 4:
            animstate->m_flGoalFeetYaw = math::normalize_yaw(player->m_angEyeAngles().y + 15.f * resolving_way );
            break;
        case 5:
            animstate->m_flGoalFeetYaw = math::normalize_yaw(player->m_angEyeAngles().y - 15.f * resolving_way );
            break;
        }
}

float resolver::resolve_pitch()
{
    return original_pitch;
}
*/