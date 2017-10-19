#include <stdint.h>
#include <stddef.h>

#define MORSE_TX_BUFFER_SIZE  10

static char tx_buffer[MORSE_TX_BUFFER_SIZE];
static uint16_t  tx_buffer_write_index = 0;
static uint16_t  tx_buffer_read_index = 0;
static uint16_t  tx_buffer_count = 0;

/*
 */
struct character
{
	char ch;
	struct character *parent;
	struct character *dot;
	struct character *dash;
};

static struct character alphabet['z' - 'a' + 1];   // Plus 1 for Root Node

#define INDEX(x)  ((x - 'a') + 1)                  // Plus 1 for Root Node

void morse_init(void)
{
	uint8_t loop;

	alphabet[0].ch = 0x00;
	
	for(loop = 'a'; loop <= 'z'; loop++) {
		alphabet[INDEX(loop)].ch = loop;
		alphabet[INDEX(loop)].parent = NULL;
		alphabet[INDEX(loop)].dot    = NULL;
		alphabet[INDEX(loop)].dash   = NULL;
	}
	
	/*
	 * The Initial element of the Array [0] is the Root Node
	 */
	alphabet[0].parent = NULL;
	alphabet[0].dot    = &alphabet[INDEX('e')];
	alphabet[0].dash   = &alphabet[INDEX('t')];

	alphabet[INDEX('a')].parent = &alphabet[INDEX('e')];
	alphabet[INDEX('a')].dot    = &alphabet[INDEX('r')];
	alphabet[INDEX('a')].dash   = &alphabet[INDEX('w')];

	alphabet[INDEX('b')].parent = &alphabet[INDEX('d')];
	
	alphabet[INDEX('c')].parent = &alphabet[INDEX('k')];
	
	alphabet[INDEX('d')].parent = &alphabet[INDEX('n')];
	alphabet[INDEX('d')].dot    = &alphabet[INDEX('b')];
	alphabet[INDEX('d')].dash   = &alphabet[INDEX('x')];

	alphabet[INDEX('e')].parent = &alphabet[0];  // Root Node
	alphabet[INDEX('e')].dot    = &alphabet[INDEX('i')];
	alphabet[INDEX('e')].dash   = &alphabet[INDEX('a')];

	alphabet[INDEX('f')].parent = &alphabet[INDEX('u')];

	alphabet[INDEX('g')].parent = &alphabet[INDEX('m')];
	alphabet[INDEX('g')].dot    = &alphabet[INDEX('z')];
	alphabet[INDEX('g')].dash   = &alphabet[INDEX('q')];

	alphabet[INDEX('h')].parent = &alphabet[INDEX('s')];

	alphabet[INDEX('i')].parent = &alphabet[INDEX('e')];
	alphabet[INDEX('i')].dot    = &alphabet[INDEX('s')];
	alphabet[INDEX('i')].dash   = &alphabet[INDEX('u')];

	alphabet[INDEX('j')].parent = &alphabet[INDEX('w')];

	alphabet[INDEX('k')].parent = &alphabet[INDEX('n')];
	alphabet[INDEX('k')].dot    = &alphabet[INDEX('c')];
	alphabet[INDEX('k')].dash   = &alphabet[INDEX('y')];

	alphabet[INDEX('l')].parent = &alphabet[INDEX('r')];

	alphabet[INDEX('m')].parent = &alphabet[INDEX('t')];
	alphabet[INDEX('m')].dot    = &alphabet[INDEX('g')];
	alphabet[INDEX('m')].dash   = &alphabet[INDEX('o')];

	alphabet[INDEX('n')].parent = &alphabet[INDEX('t')];
	alphabet[INDEX('n')].dot    = &alphabet[INDEX('d')];
	alphabet[INDEX('n')].dash   = &alphabet[INDEX('k')];

	alphabet[INDEX('o')].parent = &alphabet[INDEX('m')];

	alphabet[INDEX('p')].parent = &alphabet[INDEX('w')];
	
	alphabet[INDEX('q')].parent = &alphabet[INDEX('g')];
	
	alphabet[INDEX('r')].parent = &alphabet[INDEX('a')];
	alphabet[INDEX('r')].dot    = &alphabet[INDEX('l')];

	alphabet[INDEX('s')].parent = &alphabet[INDEX('i')];
	alphabet[INDEX('s')].dot    = &alphabet[INDEX('h')];
	alphabet[INDEX('s')].dash   = &alphabet[INDEX('v')];

	alphabet[INDEX('t')].parent = &alphabet[0];       // Root Node
	alphabet[INDEX('t')].dot    = &alphabet[INDEX('n')];
	alphabet[INDEX('t')].dash   = &alphabet[INDEX('m')];

	alphabet[INDEX('u')].parent = &alphabet[INDEX('i')];
	alphabet[INDEX('u')].dot    = &alphabet[INDEX('f')];

	alphabet[INDEX('v')].parent = &alphabet[INDEX('s')];

	alphabet[INDEX('w')].parent = &alphabet[INDEX('a')];
	alphabet[INDEX('w')].dot    = &alphabet[INDEX('p')];
	alphabet[INDEX('w')].dash   = &alphabet[INDEX('j')];

	alphabet[INDEX('x')].parent = &alphabet[INDEX('d')];

	alphabet[INDEX('y')].parent = &alphabet[INDEX('k')];

	alphabet[INDEX('z')].parent = &alphabet[INDEX('g')];
}

void morse_tx(char *msg)
{
	char *ptr = msg;
	
	while (*ptr) {
		/*
		 * Check that the tx buffer isn't full aready.
		 */
		if ( tx_buffer_count < MORSE_TX_BUFFER_SIZE ) {
			
			/*
			 * Check for a valid character
			 */
			if( (*ptr == ' ') || (*ptr >= 'a' && *ptr <= 'z') ) {
				tx_buffer[tx_buffer_write_index] = *ptr;
				tx_buffer_write_index = (tx_buffer_write_index + 1) % MORSE_TX_BUFFER_SIZE;
				tx_buffer_count++;
			}
			
			ptr++;
		}
	}
}

