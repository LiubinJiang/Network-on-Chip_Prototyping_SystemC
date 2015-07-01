#include <time.h>
#include "systemc.h"

#include "router.h"
#include "pe.h"

SC_MODULE(top)
{
public:
	enum {N = 3};

	router *routers[N];
	PE_base *pes[N];

	sc_signal<packet> router_to_pe[N], pe_to_router[N];
	sc_signal<packet> router_to_router_east[N-1], router_to_router_west[N-1];
	sc_signal<packet> terminal_loop_north[N], terminal_loop_south[N];
	sc_signal<packet> terminal_loop_east, terminal_loop_west;
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
		pes[0]->set_xy(0, 0);

		pes[1] = new PE_inc("P1");
		pes[1]->clock(clock);
		pes[1]->set_xy(1, 0);
        
        //modified
        pes[2]=new PE_inc("P2");
        pes[2]->clock(clock);
        pes[2]->set_xy(2,0);
	}

	void create_network()
	{
		for (int i = 0; i < N; ++i)
		{
			char name[100];
			sprintf(name, "router%d", i);

			// create router
			routers[i] = new router(name);
			routers[i]->set_xy(i,0);
			routers[i]->clock(clock);

			// loop unused ports
			routers[i]->port_in[router::NORTH](
				terminal_loop_north[i]);
			routers[i]->port_out[router::NORTH](
				terminal_loop_north[i]);
			routers[i]->port_in[router::SOUTH](
				terminal_loop_south[i]);
			routers[i]->port_out[router::SOUTH](
				terminal_loop_south[i]);

			// connect router to west routers
			if (i != 0)
			{
				routers[i]->port_out[router::WEST](
					router_to_router_west[i-1]);
				routers[i]->port_in[router::WEST](
					router_to_router_east[i-1]);
			}
			else // or make a loop
			{
				routers[i]->port_out[router::WEST](
					terminal_loop_west);
				routers[i]->port_in[router::WEST](
					terminal_loop_west);
			}

			if (i != N-1) // connect router to east routers
			{
				routers[i]->port_out[router::EAST](
					router_to_router_east[i]);
				routers[i]->port_in[router::EAST](
					router_to_router_west[i]);
			}
			else // or make a loop
			{
				routers[i]->port_out[router::EAST](
					terminal_loop_east);
				routers[i]->port_in[router::EAST](
					terminal_loop_east);
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
    //modified start
    //PE fired
    extern int count_PO;
    extern int count_PI;
    extern int count_P1;
    extern int count_P2;
    
    //pkts in commnication links
    extern int count_P2R_0;
    extern int count_R2P_0;
    
    extern int count_R2R_east_0;
    extern int count_R2R_west_0;
    
    extern int count_P2R_1;
    extern int count_R2P_1;
    
    //a third node
    extern int count_P2R_2;
    extern int count_R2P_2;
    extern int count_R2R_west_1;
    extern int count_R2R_east_1;
    
    extern size_t queue_PO; //PO & PI
    extern size_t queue_P1;
    extern size_t queue_R0toPO;
    extern size_t queue_R0toR1;
    extern size_t queue_R1toP1;
    extern size_t queue_R1toR0;
    
    extern size_t queue_P2;
    extern size_t queue_R1toR2;
    extern size_t queue_R2toR1;
    extern size_t queue_R2toP2;
    
    
    float cycle_number=20.0;
    
   
    
    //modified end
    
    
	srand(0);

	top top_module("top");

	printf("cycle  0 ================================\n");
	sc_start(0, SC_NS);

	for(int i = 1; i < 20; i++){
		
		printf("cycle %2d ================================\n", i);
        


		top_module.clock.write(1);
		sc_start(10, SC_NS);
		top_module.clock.write(0);
		sc_start(10, SC_NS);
	}
    
    //Modified Start
    float utilization_PO;
    float utilization_PI;
    float utilization_P1;
    float utilization_P2;
    
    
    float utilization_P2R_0;
    float utilization_R2P_0;
    float utilization_R2R_east_0;
    float utilization_R2R_west_0;
    float utilization_P2R_1;
    float utilization_R2P_1;
    
    float utilization_P2R_2;
    float utilization_R2P_2;
    float utilization_R2R_west_1;
    float utilization_R2R_east_1;
    
    utilization_PO=count_PO/cycle_number;
    utilization_PI=count_PI/cycle_number;
    utilization_P1=count_P1/cycle_number;
    utilization_P2=count_P2/cycle_number;
    
    utilization_P2R_0=count_P2R_0/cycle_number;
    utilization_R2P_0=count_R2P_0/cycle_number;
    utilization_R2R_east_0=count_R2R_east_0/cycle_number;
    utilization_R2R_west_0=count_R2R_west_0/cycle_number;
    utilization_P2R_1=count_P2R_1/cycle_number;
    utilization_R2P_1=count_R2P_1/cycle_number;
    utilization_P2R_2=count_P2R_2/cycle_number;
    utilization_R2P_2=count_R2P_2/cycle_number;
    utilization_R2R_west_1=count_R2R_west_1/cycle_number;
    utilization_R2R_east_1=count_R2R_east_1/cycle_number;


    
    printf("\n+++++++++SUMMARY_UTILIZATION+++++++++++++\n");
    /*printf("PO fired %d times",count_PO);
    printf("PI fired %d times",count_PI);
    printf("P1 fired %d times",count_P1);*/
    std::cerr<<"PO -> "<<utilization_PO<<std::endl;
    std::cerr<<"PI -> "<<utilization_PI<<std::endl;
    std::cerr<<"P1 -> "<<utilization_P1<<std::endl;
    std::cerr<<"P2 -> "<<utilization_P2<<std::endl;
    
    std::cerr<<"pe to router [0] -> "<<utilization_P2R_0<<std::endl;
    std::cerr<<"router to pe [0] -> "<<utilization_R2P_0<<std::endl;
    std::cerr<<"pe to router [1] -> "<<utilization_P2R_1<<std::endl;
    std::cerr<<"router to pe [1] -> "<<utilization_R2P_1<<std::endl;
    std::cerr<<"pe to router [2] -> "<<utilization_P2R_2<<std::endl;
    std::cerr<<"router to pe [2] -> "<<utilization_R2P_2<<std::endl;
    std::cerr<<"router to router west [0] -> "<<utilization_R2R_west_0<<std::endl;
    std::cerr<<"router to router east [0] -> "<<utilization_R2R_east_0<<std::endl;
    std::cerr<<"router to router west [1] -> "<<utilization_R2R_west_1<<std::endl;
    std::cerr<<"router to router east [1] -> "<<utilization_R2R_east_1<<std::endl;

    
    printf("\n+++++++++SUMMARY_QUEUE SIZE+++++++++++++\n");
    std::cerr<<"queue size of PO-> "<<queue_PO<<std::endl;
    std::cerr<<"queue size of P1-> "<<queue_P1<<std::endl;
    std::cerr<<"queue size of P2-> "<<queue_P2<<std::endl;
    std::cerr<<"queue size of router[0,0] to PO-> "<<queue_R0toPO<<std::endl;
    std::cerr<<"queue size of router[0,0] to router[1,0]-> "<<queue_R0toR1<<std::endl;
    std::cerr<<"queue size of router[1,0] to P1-> "<<queue_R1toP1<<std::endl;
    std::cerr<<"queue size of router[1,0] to router[0,0]-> "<<queue_R1toR0<<std::endl;
    std::cerr<<"queue size of router[2,0] to P2-> "<<queue_R2toP2<<std::endl;
    std::cerr<<"queue size of router[2,0] to router[1,0]-> "<<queue_R2toR1<<std::endl;
    std::cerr<<"queue size of router[1,0] to router[2,0]-> "<<queue_R1toR2<<std::endl;
    
   
    
    
    //Modified end

	return 0;
}
