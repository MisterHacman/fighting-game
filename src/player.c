#include "player.h"

Error initPlayer(Player *player, PlayerId player_id, SDL_Renderer *renderer) {
	printf("initializing player\n\n");
	int error;

	Player player_data = {
		.id = player_id,
		.head = {.x = 0, .y = 0, .xvel = 0, .yvel = 0},
		.dir = 1,
		.attached_to_gamepad = 0,
		.controller = DEFAULT_CONTROLLER,
		.control_status = { 0, 0, 0, 0, 0, 0 },
	};
	*player = player_data;

	if ((error = initPlayerConstants(player, player_id))) return error;

	return SUCCESS;
}

Error initPlayerConstants(Player *player, PlayerId player_id) {
	printf("initializing player constants\n");
	if (player_id == CAT) {
	}
	return SUCCESS;
}

Error updatePlayer(Player *player) {
	//printf("updating player\n");

	return SUCCESS;
}

Error drawPlayer(Player player, SDL_Renderer *renderer) {
	return SUCCESS;
}

void freePlayer(Player *player) {
	printf("freeing player\n");
	//SDL_DestroyTexture(player->textures);
	printf("\n");
}
