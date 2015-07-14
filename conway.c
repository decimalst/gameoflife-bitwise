// Basic version of Conway's game of life
// Author: Byron Lambrou
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Define a boolean variable type
//typedef enum { false, true } bool;

int i = 0;
int j = 0;
int k = 0;
// If 1, we modify world_calc
int is_world_calc = 1;
long tick_count = 0;
uint32_t *world_calc;
uint32_t *world_write;

const int number_of_rows = 32;
//for (i=0; i<number_of_rows; i++)
//{
	// Initialize the rows to zero
//	world_calc[i] = 0;
//	world_write[i] = 0;
//}

void init()
{
	world_calc = calloc( 32, sizeof(uint32_t) ); //Allocate 32 rows for world
	world_write = calloc( 32, sizeof(uint32_t) ); //Allocate 32 rows for world
}

//Function pre declarations
int check_cell(int, int, int);
void display_world();
void update_draw_world ();
void memory_cleanup ();
void conway_test();
void printBits(size_t const, void const * const);
void on_tick();
void write_cell(int, int, int);



int main()
{
	init();

	// Main Game running loop
	// while (1){
	// 	//printf("%d \n", *(world_calc+1));
	// 	display_world();
	// }
	conway_test();

	memory_cleanup();
}

void on_tick()
{
	// Function to be called once per tick.
	// Every tick we should go through the world and run check_cell on each cell in
	// world_write to determine the new value of world_calc's cells
	// Then, we set world_write's values equal to world_calc's new value and display them
	for (j=0; j<number_of_rows; j++)
	{
		for (k=0; k<32; k++){
			write_cell( j, k, check_cell( j, k, is_world_calc) );
		}
	}
	display_world();
	is_world_calc = !(is_world_calc);
}

void write_cell(int x, int y, int live_neighbors)
{
	uint32_t* which_world;
	uint32_t* other_world;
	int was_dead;
	if(is_world_calc){
		which_world=world_write;
		other_world=world_calc;
	}
	else{
		which_world=world_calc;
		other_world=world_write;
	}
	if(*(which_world + y) & ( 1 << x) ){
		printf("x=%i y=%i live_neighbor %i & was not dead \n",x,y, live_neighbors);
		was_dead=0;
	}
	else{
		printf("x=%i y=%i live %i & was dead\n",x,y, live_neighbors);
		was_dead=1;
	}
	if ((live_neighbors == 2 && !was_dead )|| (live_neighbors == 3 && was_dead))
	{
		*(which_world + y) = *(other_world + y) | ( 1 << x);
	}
	else {
		*(which_world + y) = *(other_world + y) & ( 0xFFFFFFFF - ( 1 << x ) );
	}
}
int check_cell(int x, int y, int which_world)
{
	// Checks what should happen to a cell in the next generation
	// Returns 0 if cell should die, returns 1 if the cell should live

	uint32_t cell_x = x;
	uint32_t cell_y = y;
	int return_val;
	int live_neightbors_count = 0;
	uint32_t *modified_world;
	if (which_world){
		modified_world=world_calc;
	}
	else{
		modified_world=world_write;
	}
	// First, check x neighbors to the left and right
	// Check left and right
	switch (cell_x) 
	{
		case 31 :
			// X is in the right-most column, so wrap around
			if( *( modified_world + cell_y ) & ( 1 << ( 30 )) )
			{
				live_neightbors_count++;
			}
			if( *( modified_world + cell_y ) & ( 1 ) )
			{
				live_neightbors_count++;
			}
			break;
		case 1 :
			// X is in the left-most column, so wrap around
			if( *( modified_world + cell_y ) & ( 1 << 1 ) )
			{
				live_neightbors_count++;
			}
			// 2^ 31 = 2147483648 = 0x80000000
			// Or 0b10000000000000000000000000000000
			if( *( modified_world + cell_y ) & ( 0x80000000 ) )
			{
				live_neightbors_count++;
			}
			break;
		default :
			// check for neightbors normally
			if( *( modified_world + cell_y ) & ( 1 << (cell_x + 1) ) )
			{
				live_neightbors_count++;
			}
			if( *( modified_world + cell_y ) & ( 1 << (cell_x -1) ) )
			{
				live_neightbors_count++;
			}
			break;
	}
	// Check up and down
	switch (cell_y)
	{
		case 31 :
			// Y is in the bottom-most column, so wrap around
			if( *( modified_world ) & ( 1 << cell_x ) )
			{
				live_neightbors_count++;
			}
			if( *( modified_world + 30 ) & ( 1 << cell_x ) )
			{
				live_neightbors_count++;
			}
			break;
		case 0 :
			// Y is in the top-most column, so wrap around
			if( *( modified_world + 1 ) & ( 1 << cell_x ) )
			{
				live_neightbors_count++;
			}
			// 2^ 31 = 2147483648
			// Or 0b10000000000000000000000000000000
			if( *( modified_world + 31 ) & ( 1 << cell_x ) )
			{
				live_neightbors_count++;
			}
			break;
		default :
			// check for neightbors normally
			if( *( modified_world + (cell_y + 1 )) & ( 1 << cell_x ) )
			{
				live_neightbors_count++;
			}
			if( *(modified_world + (cell_y - 1 )) & ( 1 << cell_x ) )
			{
				live_neightbors_count++;
			}
			break;
	}
	// Check diagonals
	switch (cell_x)
	{
		case 31 :
			switch (cell_y)
			{
				case 31 :
					if( *( modified_world ) & ( 1 << 30 ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world ) & ( 1 ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + 30 ) & ( 1 ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + 30 ) & ( 1 << 30 ) )
					{
						live_neightbors_count++;
					}
					break;
				case 0 :
					if( *( modified_world + 1) & ( 1 << 30 ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + 1) & ( 1 ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + 31 ) & ( 1 ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + 31 ) & ( 1 << 30 ) )
					{
						live_neightbors_count++;
					}
					break;
				default :
					if( *( modified_world + (cell_y + 1 )) & ( 1 << 30 ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world +  (cell_y + 1)) & ( 1 ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + (cell_y -1 )) & ( 1 ) )
					{
						live_neightbors_count++;
					}
					if( *(modified_world + ( cell_y - 1 )) & ( 1 << 30 ) )
					{
						live_neightbors_count++;
					}
					break;

			}
		case 0 :
			switch (cell_y)
			{
				case 31:
					if( *( modified_world ) & ( 1 << 31 ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world ) & ( 2 ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + 30 ) & ( 2 ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + 30 ) & ( 1 << 31 ) )
					{
						live_neightbors_count++;
					}
					break;
				case 0 :
					if( *( modified_world + 1) & ( 1 << 31 ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + 1) & ( 2 ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + 31 ) & ( 2 ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + 31 ) & ( 1 << 31 ) )
					{
						live_neightbors_count++;
					}
					break;
				default :
					if( *( modified_world + (cell_y + 1)) & ( 1 << 31) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + (cell_y + 1)) & ( 1 << 1 ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + (cell_y - 1 )) & ( 1 << 31 ) )
					{
						live_neightbors_count++;
					}
					if( *(modified_world + ( cell_y - 1 )) & ( 1 << 1) )
					{
						live_neightbors_count++;
					}
					break;

			}
		default :
			switch (cell_y)
			{
				case 31:
					if( *( modified_world + 30 ) & ( 1 << (cell_x + 1) ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world ) & ( 1 << (cell_x - 1) ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + 30 ) & ( 1 << (cell_x - 1) ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world) & ( 1 << (cell_x + 1) )  )
					{
						live_neightbors_count++;
					}
					break;
				case 0:
					if( *( modified_world + 1 ) & ( 1 << (cell_x + 1) ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + 31 ) & ( 1 << (cell_x - 1) ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + 1 ) & ( 1 << (cell_x - 1) ) ) 
					{
						live_neightbors_count++;
					}
					if( *( modified_world + 31 ) & ( 1 << (cell_x + 1) )  )
					{
						live_neightbors_count++;
					}
					break;
				default :
					if( *( modified_world + (cell_y + 1) ) & ( 1 << (cell_x + 1) ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + (cell_y - 1) ) & ( 1 << (cell_x - 1) ) )
					{
						live_neightbors_count++;
					}
					if( *( modified_world + (cell_y + 1) ) & ( 1 << (cell_x - 1) ) ) 
					{
						live_neightbors_count++;
					}
					if( *( modified_world + (cell_y - 1) ) & ( 1 << (cell_x + 1) )  )
					{
						live_neightbors_count++;
					}
					break;
			}
	}
	return live_neightbors_count;
}

void display_world ()
{
	// Draws world to the screen
	for (i=0; i<32; i++)
	{
		if(is_world_calc){
			printBits(sizeof(uint32_t), (world_calc+i));
		}
		else{
			printBits(sizeof(uint32_t), (world_write+i));
		}
	}
	printf("--------------------------------\n");
}

void memory_cleanup ()
{
	// Frees memory on the heap
	free(world_write);
	free(world_calc);
}
void neighbors_test () {
	for (j=0; j<number_of_rows; j++)
	{
		for (k=0; k<32; k++){
			int derp=check_cell( j, k, is_world_calc);
			printf("%i", derp);
		}
		printf("\n");
	}
	printf("-------\n");
}

void conway_test()
{

	write_cell(2, 2, 3);
	write_cell(3, 2, 3);
	is_world_calc=!is_world_calc;
	neighbors_test();
	display_world();
	
	//write_cell(3, 3, 2);
	//display_world();
	printf("%i \n",check_cell(2, 2, 1));
	printf("%i \n",check_cell(3, 2, 1));
	printf("%i \n",check_cell(4, 2, 1));
	printf("%i \n",check_cell(3, 1, 1));
	//on_tick();
	// on_tick();
	// on_tick();
	// on_tick();
}

// This function taken from stackexchange
// assumes little endian
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = b[i] & (1<<j);
            byte >>= j;
            printf("%u", byte);
        }
    }
    puts("");
}