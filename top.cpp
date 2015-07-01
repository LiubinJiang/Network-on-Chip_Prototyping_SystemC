#include <time.h>
#include "systemc.h"

#include "router.h"
#include "pe.h"

SC_MODULE(top)
{
public:
	enum {N = 9};

	router *routers[N];
	PE_base *pes[N];

	sc_signal<packet> router_to_pe[N], pe_to_router[N];
	sc_signal<packet> router_to_router_east[N-3], router_to_router_west[N-3], router_to_router_north[N-3], router_to_router_south[N-3];
	sc_signal<packet> terminal_loop_north[N - 6], terminal_loop_south[N - 6], terminal_loop_east[N - 6], terminal_loop_west[N - 6];
	//sc_signal<packet> 
	sc_signal<bool> clock;
    
    //modified

	SC_CTOR(top)
	{
		create_pes();
		create_network();
	}

protected:
	void create_pes()
	{
		pes[0] = new PE_IO("PI/PO");
		pes[0]->clock(clock);
		pes[0]->set_xy(1, 1);
		
		pes[1] = new PE_inc("P1");
		pes[1]->clock(clock);
		pes[1]->set_xy(0, 0);
        
        //modified
        pes[2]=new PE_inc("P2");
        pes[2]->clock(clock);
        pes[2]->set_xy(1,0);

		pes[3] = new PE_inc("P3");
		pes[3]->clock(clock);
		pes[3]->set_xy(2, 0);

		pes[4] = new PE_inc("P4");
		pes[4]->clock(clock);
		pes[4]->set_xy(0, 1);

		pes[5] = new PE_inc("P5");
		pes[5]->clock(clock);
		pes[5]->set_xy(2, 1);

		pes[6] = new PE_inc("P6");
		pes[6]->clock(clock);
		pes[6]->set_xy(0, 2);

		pes[7] = new PE_inc("P7");
		pes[7]->clock(clock);
		pes[7]->set_xy(1, 2);

		pes[8] = new PE_inc("P8");
		pes[8]->clock(clock);
		pes[8]->set_xy(2, 2);
	}

	void create_network()
	{
		for (int i = 0; i < N; ++i)
		{
			int router_x = i % 3;
			int router_y = i / 3;
			char name[100];
			sprintf(name, "router%d", i);

			// create router
			routers[i] = new router(name);
			//proj2 modified
			routers[i]->set_xy(router_x , router_y);
			//routers[i]->set_xy(i,0);
			routers[i]->clock(clock);

			// loop unused ports
			/*routers[i]->port_in[router::NORTH](
				terminal_loop_north[i]);
			routers[i]->port_out[router::NORTH](
				terminal_loop_north[i]);
			routers[i]->port_in[router::SOUTH](
				terminal_loop_south[i]);
			routers[i]->port_out[router::SOUTH](
				terminal_loop_south[i]);*/

			// connect router to west routers
			// proj2 modifed i
			if (router_x != 0)
			{
				routers[i]->port_out[router::WEST](
					router_to_router_west[i-1-router_y]);
				routers[i]->port_in[router::WEST](
					router_to_router_east[i-1-router_y]);
			}
			else // or make a loop
			{
				routers[i]->port_out[router::WEST](
					terminal_loop_west[router_y]);
				routers[i]->port_in[router::WEST](
					terminal_loop_west[router_y]);
			}

			if (router_x != 2) // connect router to east routers
			{
				routers[i]->port_out[router::EAST](
					router_to_router_east[i-router_y]);
				routers[i]->port_in[router::EAST](
					router_to_router_west[i-router_y]);
			}
			else // or make a loop
			{
				routers[i]->port_out[router::EAST](
					terminal_loop_east[router_y]);
				routers[i]->port_in[router::EAST](
					terminal_loop_east[router_y]);
			}

			if (router_y != 0) //connect router to north routers
			{
				routers[i]->port_out[router::NORTH](router_to_router_north[i - 3]);
				routers[i]->port_in[router::NORTH](router_to_router_south[i - 3]);
			}
			else
			{
				routers[i]->port_out[router::NORTH](terminal_loop_north[router_x]);
				routers[i]->port_in[router::NORTH](terminal_loop_north[router_x]);
			}
			if (router_y != 2)//connect router to south routers
			{
				routers[i]->port_out[router::SOUTH](router_to_router_south[i]);
				routers[i]->port_in[router::SOUTH](router_to_router_north[i]);
			}
			else
			{
				routers[i]->port_out[router::SOUTH](terminal_loop_south[router_x]);
				routers[i]->port_in[router::SOUTH](terminal_loop_south[router_x]);
			}

			// connect router to PE 
			routers[i]->port_out[router::PE](router_to_pe[i]);
			routers[i]->port_in[router::PE](pe_to_router[i]);
			pes[i]->data_in(router_to_pe[i]);
			pes[i]->data_out(pe_to_router[i]);
		}
	}

}; // top

int sc_main(int argc , char *argv[])
{
	srand(0);

	top top_module("top");

	printf("cycle  0 ================================\n");
	sc_start(0, SC_NS);
	//getchar();
	for(int i = 1; i < 200; i++){
		
		printf("cycle %2d ================================\n", i);
		top_module.clock.write(1);
		sc_start(10, SC_NS);
		top_module.clock.write(0);
		sc_start(10, SC_NS);
	}
	
	return 0;
}
