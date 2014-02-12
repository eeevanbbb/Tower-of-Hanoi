//hanoi2.c - Tower of Hanoi game

#import <stdlib.h>
#import <stdio.h>
#import <string.h>
#import <stdbool.h>

typedef struct disk_store {
	int radius;
} *disk;

disk new_disk(int radius) {
	disk result = (disk) malloc(sizeof(struct disk_store));
	result->radius = radius;
	return result;
}

typedef struct disk_cons_store {
	disk contents;
	struct disk_cons_store *below;
} *disk_cons;

disk_cons new_disk_cons(disk contents) {
	disk_cons result = (disk_cons) malloc(sizeof(struct disk_cons_store));
	result->contents = contents;
	return result;
}

typedef struct tower_store {
	disk_cons top;
	disk_cons bottom;
	int count;
} *tower;

tower new_tower() {
	tower result = (tower) malloc(sizeof(struct tower_store));
	result->count = 0;
	return result;
}

void add_to_tower(tower theTower, disk theDisk) {
	
	disk_cons newDiskCons = new_disk_cons(theDisk);
	
	if (!theTower->bottom) {
		theTower->bottom = newDiskCons;
		theTower->top = newDiskCons;
	}
	else {
		newDiskCons->below = theTower->top;
		theTower->top = newDiskCons;
	}
	theTower->count++;
}

disk remove_from_tower(tower theTower) {

	disk_cons result = theTower->top;
	if (result) {
		theTower->top = result->below;
		if (!result->below) {
			theTower->bottom = NULL;
		}
		return result->contents;
	}
	return NULL;
}

bool can_add_to_tower(tower theTower, disk toAdd) {
	if (theTower->top) {
		return theTower->top->contents->radius > toAdd->radius;
	}
	return true;
}

bool can_remove_from_tower(tower theTower) {
	if (theTower->top) {
		return true;
	}
	return false;
}

tower tower1;
tower tower2;
tower tower3;

void set_up_game(int howMany) {

	tower1 = new_tower();
	tower2 = new_tower();
	tower3 = new_tower();

	for (int i = 0; i<howMany; i++) {
		disk newDisk = new_disk(howMany-i);
		add_to_tower(tower1, newDisk);
	}
}

char * get_user_input() {	
	char *input_string = malloc(1024);
	fgets(input_string,1024,stdin); //get the user input from stdin
	return input_string;
}

bool is_game_over() {
	return (!tower1->bottom && !tower2->bottom) || (!tower1->bottom && !tower3->bottom);
}

void print_tower(tower theTower) {
	disk_cons current = theTower->top;
	
	while(current) {
		printf(" %d",current->contents->radius);
		current = current->below;
	}
}

//This isn't a good layout. Ideally, the towers should be printed vertically, ya know, like towers.
void print_state() {
	printf("Tower 1:");
	print_tower(tower1);
	printf("\nTower 2:");
	print_tower(tower2);
	printf("\nTower 3:");
	print_tower(tower3);
	printf("\n");
}

void game_loop() {
	print_state();
	
	printf("Move from tower: ");
	int fromTowerInt = atoi(get_user_input());
	printf("To tower: ");
	int toTowerInt = atoi(get_user_input());
	
	tower toTower;
	tower fromTower;
	
	switch(fromTowerInt) {
		case 1:
			fromTower = tower1;
			break;
		case 2:
			fromTower = tower2;
			break;
		case 3:
			fromTower = tower3;
			break;
	}
	switch(toTowerInt) {
		case 1:
			toTower = tower1;
			break;
		case 2:
			toTower = tower2;
			break;
		case 3:
			toTower = tower3;
			break;
	}
	
	if (can_remove_from_tower(fromTower)) {
		disk movingDisk = remove_from_tower(fromTower);
		if (can_add_to_tower(toTower,movingDisk)) {
			add_to_tower(toTower,movingDisk);
		}
		else {
			add_to_tower(fromTower,movingDisk);
			printf("Illegal move. Tried to place bigger disk on smaller disk.\n");
		}
	}
	else {
		printf("Illegal move. No disks on tower.\n");
	}
	
	if (is_game_over()) {
		print_state();
		printf("You win!\n");
	}
	else {
		game_loop();
	}
}


int main(int argc, char **argv) {

	printf("How many disks? > ");
	set_up_game(atoi(get_user_input()));
	
	game_loop();

	exit(0);
}