/* ****************************************************
Gameplay
The vertices of a dodecahedron illustrate one common shape of the
labyrinth in the Hunt the Wumpus game.

The original text-based version of Hunt the Wumpus uses a command line
text interface. A player of the game enters commands to move through
the rooms or to shoot "crooked arrows" along a tunnel into one of
the adjoining rooms. There are twenty rooms, each connecting to three
others, arranged like the vertices of a dodecahedron or the faces of an
icosahedron (which are identical in layout). Hazards include bottomless
pits, super bats (which drop the player in a random location, a feature
duplicated in later, commercially published adventure games, such as Zork
I, Valley of the Minotaur, and Adventure), and the Wumpus itself. The
Wumpus is described as having sucker feet (to escape the bottomless pits)
and being too heavy for a super bat to lift. When the player has deduced
from hints which chamber the Wumpus is in without entering the chamber,
the player fires an arrow into the Wumpus's chamber to kill it. The
player wins the game if the Wumpus is killed. However, firing the arrow
into the wrong chamber startles the Wumpus, which may cause it to move
to an adjacent room. The player loses if he or she is in the same room
as the Wumpus (which then eats him or her) or a bottomless pit (which
he or she then falls into).
Game elements

Yob's original program had these features, while later programs differ
here.

    Objects:
        Wumpus: your target; a beast that eats you if you ever end up
        in the same room.
        Super Bats (2): creatures that instantly carry you to a random
        room.
        Pits (2): fatal to you if you enter the room.
    Actions: There are two possible actions:
        Move: to one of the three rooms connected to your current one.
        Shoot: fire a "crooked arrow" a distance of 1-5 rooms; you must
        name each room it will reach.
    Warning messages: Give you information about the contents of
    adjacent rooms.
        Wumpus: "I smell a wumpus"
        Bat: "Bats nearby"
        Pit: "I feel a draft"

 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define ROOM_A      (0)
#define ROOM_B      (1)
#define ROOM_C      (2)
#define ROOM_D      (3)
#define ROOM_E      (4)
#define ROOM_F      (5)
#define ROOM_G      (6)
#define ROOM_H      (7)
#define ROOM_I      (8)
#define ROOM_J      (9)
#define ROOM_K      (10)
#define ROOM_L      (11)
#define ROOM_M      (12)
#define ROOM_N      (13)
#define ROOM_O      (14)
#define ROOM_P      (15)
#define ROOM_Q      (16)
#define ROOM_R      (17)
#define ROOM_S      (18)
#define ROOM_T      (19)
#define NUM_ROOMS   (20)

////////////////////////////////////////////////////////////////////////////

static struct {
   uint8_t room;
   uint8_t exits[3];
} g_map[] = {
   { ROOM_A, { ROOM_P, ROOM_B, ROOM_J } },
   { ROOM_B, { ROOM_A, ROOM_L, ROOM_C } },
   { ROOM_C, { ROOM_B, ROOM_Q, ROOM_D } },
   { ROOM_D, { ROOM_C, ROOM_M, ROOM_E } },
   { ROOM_E, { ROOM_D, ROOM_R, ROOM_F } },
   { ROOM_F, { ROOM_E, ROOM_N, ROOM_G } },
   { ROOM_G, { ROOM_F, ROOM_S, ROOM_H } },
   { ROOM_H, { ROOM_G, ROOM_O, ROOM_I } },
   { ROOM_I, { ROOM_H, ROOM_T, ROOM_J } },
   { ROOM_J, { ROOM_I, ROOM_K, ROOM_A } },
   { ROOM_K, { ROOM_J, ROOM_O, ROOM_L } },
   { ROOM_L, { ROOM_K, ROOM_B, ROOM_M } },
   { ROOM_M, { ROOM_L, ROOM_D, ROOM_N } },
   { ROOM_N, { ROOM_M, ROOM_F, ROOM_O } },
   { ROOM_O, { ROOM_N, ROOM_H, ROOM_K } },
   { ROOM_P, { ROOM_T, ROOM_A, ROOM_Q } },
   { ROOM_Q, { ROOM_P, ROOM_C, ROOM_R } },
   { ROOM_R, { ROOM_Q, ROOM_E, ROOM_S } },
   { ROOM_S, { ROOM_R, ROOM_G, ROOM_T } },
   { ROOM_T, { ROOM_S, ROOM_I, ROOM_P } },
};

static const uint8_t *room_exits (uint8_t room)
{
   for (size_t i=0; i<sizeof g_map/sizeof g_map[0]; i++) {
      if (room==g_map[i].room) {
         return g_map[i].exits;
      }
   }
   fprintf (stderr, "ERROR - room not found (%u not a room)\n", room);
   return NULL;
}

////////////////////////////////////////////////////////////////////////////

static struct {
   uint8_t room;
   const char *name;
} g_room_names[] = {
   // TODO: For now we use simple letter of the alphabet names, later on
   // we can give each room a decent unique name, possibly linked to
   // Lovecraft.

   { ROOM_A, "ROOM_A" },
   { ROOM_B, "ROOM_B" },
   { ROOM_C, "ROOM_C" },
   { ROOM_D, "ROOM_D" },
   { ROOM_E, "ROOM_E" },
   { ROOM_F, "ROOM_F" },
   { ROOM_G, "ROOM_G" },
   { ROOM_H, "ROOM_H" },
   { ROOM_I, "ROOM_I" },
   { ROOM_J, "ROOM_J" },
   { ROOM_K, "ROOM_K" },
   { ROOM_L, "ROOM_L" },
   { ROOM_M, "ROOM_M" },
   { ROOM_N, "ROOM_N" },
   { ROOM_O, "ROOM_O" },
   { ROOM_P, "ROOM_P" },
   { ROOM_Q, "ROOM_Q" },
   { ROOM_R, "ROOM_R" },
   { ROOM_S, "ROOM_S" },
   { ROOM_T, "ROOM_T" },

};

static const char *room_name (uint8_t room)
{
   for (size_t i=0; i<sizeof g_room_names/sizeof g_room_names[0]; i++) {
      if (room==g_room_names[i].room) {
         return g_room_names[i].name;
      }
   }
   static char errstring[45];
   sprintf (errstring, "Hell level %u can't be entered\n", room);
   return errstring;
}

static uint8_t room_number (const char *name)
{
   for (size_t i=0; i<sizeof g_room_names/sizeof g_room_names[0]; i++) {
      if ((strcmp (name, g_room_names[i].name))==0) {
         return g_room_names[i].room;
      }
   }
   return 0xff;
}

////////////////////////////////////////////////////////////////////////////
uint8_t location_player;
uint8_t location_wumpus;
uint8_t location_sbat1;
uint8_t location_sbat2;
uint8_t location_pit1;
uint8_t location_pit2;

bool wumpus_dead = false;

static void locations_init (void)
{
   // TODO: Change this to real rand - for testing everything needs to be
   // reproducable.
   srand (0);

   location_player = random () % NUM_ROOMS;
   location_wumpus = random () % NUM_ROOMS;
   location_sbat1  = random () % NUM_ROOMS;
   location_sbat2  = random () % NUM_ROOMS;
   location_pit1   = random () % NUM_ROOMS;
   location_pit2   = random () % NUM_ROOMS;
}

// Returns:
//    true  == Game is active
//    false == Game is over
static bool game_engine (void)
{
   const uint8_t *exits = room_exits (location_player);

   if (wumpus_dead) {
      printf (
            "Congratulations. You have succeeded in edging the already\n"
            "endangered Wumpus species one step forward on the path to\n"
            "extinction.\n\nThe Wumpus is dead, thanks to you.\n\n"
            "(How do you sleep at night?)\n");
      return false;
   }

   printf ("---------------------------------------------\n");
   printf ("You are in room %s\n", room_name (location_player));
   printf ("There are exits to rooms %s, %s and %s\n",
               room_name (exits[0]),
               room_name (exits[1]),
               room_name (exits[2]));

   if (location_player==location_wumpus) {
      printf (
            "You have walked in on the Wumpus. Quite embarrassingly for the "
            "Wumpus\n"
            "(and somewhat awkwardly for you) the Wumpus was apparently\n"
            "engaged in \"Wumpus Happy Time\". The Wumpus is understandably\n"
            "not happy to see you.\n\n"
            "The Wumpus lurches forwards and grabs you, foiling your attempt\n"
            "to apologise for not knocking while backing out of the room.\n"
            );
      printf ("You have been eaten by the Wumpus.\n");
      printf ("You will be sorely missed when the wumpus gets hungry again!\n");
      return false;
   }

   if (location_player==location_sbat1) {
      printf (
            "Superbat Bruce grabs you by the shoulders in a firm but loving\n"
            "grip. With a flap of his wings he lifts you off the ground and\n"
            "flies you away. Today is your day for a free ride to a random\n"
            "room.\n"
            );
      location_player = random () % NUM_ROOMS;
      location_sbat1 = random () % NUM_ROOMS;
      return game_engine ();
   }

   if (location_player==location_sbat2) {
      printf (
            "Superbat Wayne screeches at you when you enter his room. This\n"
            "invasion of his privacy will not be tolerated! In a fit of rage\n"
            "he grabs you by your nipples and ascends into the air. Mumbling\n"
            "under his breath about a joker disturbing his rest, the large\n"
            "Superbat flies on and deposits you in a random room.\n"
            );
      location_player = random () % NUM_ROOMS;
      location_sbat2 = random () % NUM_ROOMS;
      return game_engine ();
   }

   if (location_player==location_pit1) {
      printf (
            "You fall into a pit. Luckily there is no Sarlacc monster at the\n"
            "bottom of of this pit. Unluckily, there is no bottom to this pit\n"
            "either, and so you continue falling until you die of starvation.\n"
            );
      return false;
   }

   if (location_player==location_pit2) {
      printf (
            "You fall into the abyss. As you fall, you can't help but notice\n"
            "that the abyss resolutely failed to fall back into you. You\n"
            "continue falling until you starve to death.\n"
            );
      return false;
   }

   for (size_t i=0; i<3; i++) {
      if (location_wumpus==exits[i]) {
         printf ("*** You smell a wumpus.\n");
      }
      if (location_pit1==exits[i] || location_pit2==exits[i]) {
         printf ("*** You feel a draft.\n");
      }
      if (location_sbat1==exits[i] || location_sbat2==exits[i]) {
         printf ("*** You hear the flapping of leathery wings.\n");
      }
   }
   return true;
}

#define ACTION_MOVE        1
#define ACTION_SHOOT       2

struct action_t {
   uint8_t action;
   uint8_t room_args[5];
};

bool input_parse (char *input, struct action_t *dst)
{
   memset (dst, 0, sizeof *dst);

   char *tok = strtok (input, " ");
   if (!tok)
      return false;

   if ((strcmp (tok, "move")) == 0) {
      dst->action = ACTION_MOVE;
   }

   if ((strcmp (tok, "fire")) == 0) {
      dst->action = ACTION_SHOOT;
   }

   if (dst->action==0)
      return false;

   memset (dst->room_args, 0xff, sizeof dst->room_args);

   tok = strtok (NULL, " ");
   size_t idx = 0;
   while (tok && idx<5) {
      dst->room_args[idx] = room_number (tok);
      if (dst->room_args[idx] == 0xff) {
         memset (dst, 0, sizeof *dst);
         memset (dst->room_args, 0xff, sizeof dst->room_args);
         return false;
      }
      tok = strtok (NULL, " ");
      idx++;
   }

   return true;
}

bool fire_arrow (uint8_t location, uint8_t *path)
{
   if (location==location_wumpus) {
      printf ("Arrow hit Wumpus in room %s\n", room_name (location_wumpus));
      return true;
   }

   if (location==0xff || !path || path[0]==0xff) {
      printf ("Arrow reached the end of its travel, missing everything \n");
      return false;
   }

   const uint8_t *exits = room_exits (location);

   for (size_t i=0; i<3; i++) {
      if (path[0] == exits[i]) {
         printf ("Arrow enters %s\n", room_name (path[0]));
         return fire_arrow (path[0], &path[1]);
      }
   }

   printf ("Arrow falls harmlessly in %s\n", room_name (location));
   printf ("(There was exit to %s from this room\n", room_name (path[0]));

   printf ("ERROR\n");
   return false;
}

void move_player (uint8_t room)
{
   const uint8_t *exits = room_exits (location_player);

   for (size_t i=0; i<3; i++) {
      if (room==exits[i]) {
         location_player = room;
         return;
      }
   }

   printf ("Sorry, there is no exit to [%s] from [%s]\n",
            room_name (room), room_name (location_player));
}

static void print_missed_msg (void)
{
   printf ("You missed. After cursing yourself as a bungling idiot, you\n"
           "realise that the Wumpus, startled out of its wumpiuous activities\n"
           "has fled to an adjacent room.");
}

int main (void)
{
   printf ("Hunt the Wumpus\n");

   locations_init ();

   while (game_engine () == true) {
      static char input[1024];
      if (fgets (input, sizeof input - 1, stdin)==NULL) {
         printf ("Adventurer decided against hunting the Wumpus.\n");
         break;
      }

      char *tmp = strchr (input, '\n');
      if (tmp)
         *tmp=0;

      struct action_t action;
      if ((input_parse (input, &action))==false) {
         printf ("That command makes no sense.\n");
         continue;
      }

      bool killed = true;

      switch (action.action) {

         case ACTION_MOVE:    move_player (action.room_args[0]);
                              break;

         case ACTION_SHOOT:   killed = fire_arrow
                                          (location_player, action.room_args);
                              if (killed) {
                                 wumpus_dead = true;
                              } else {
                                 print_missed_msg ();
                                 const uint8_t *exits =
                                    room_exits (location_wumpus);
                                 location_wumpus = exits [random () % 3];
                              }
                              break;
      }
   }

   return EXIT_SUCCESS;
}

