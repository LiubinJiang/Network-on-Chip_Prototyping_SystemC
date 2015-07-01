#include "router.h"

int count_R2P_0=0;

int count_R2R_east_0=0;
int count_R2R_west_0=0;
int count_R2R_east_1=0;
int count_R2R_west_1=0;


int count_R2P_1=0;
int count_R2P_2=0;

size_t queue_R0toPO=0;
size_t queue_R0toR1=0;
size_t queue_R1toP1=0;
size_t queue_R1toR0=0;

size_t queue_R2toP2=0;
size_t queue_R2toR1=0;
size_t queue_R1toR2=0;

std::ostream &operator<<(std::ostream &o, const packet &p)
{
	char buf[100];
	sprintf(buf, "(%d,%d)->(%d,%d)",
		p.src_x, p.src_y, p.dest_x, p.dest_y);
	return o << buf << ", " << p.token;
}

void sc_trace(sc_trace_file *tf, const packet &p, const std::string &name)
{
	sc_trace(tf, p.src_x, name+".src.x");
	sc_trace(tf, p.src_y, name+".src.y");
	sc_trace(tf, p.dest_x, name+".dest.x");
	sc_trace(tf, p.dest_y, name+".dest.y");
	sc_trace(tf, p.token, name+".token");
}


void router::main()
{
	assert((x_ != -1) && (y_ != -1)); // to identify PE  validation

	for (int iport = 0; iport < PORTS; ++iport)
		read_packet(iport);

	for (int iport = 0; iport < PORTS; ++iport)
		write_packet(iport);
}

void router::set_xy(int x, int y)
{
	assert((x_ == -1) && (y_ == -1)); // set once only
	assert((x != -1) && (y != -1)); // must use a legal location

	x_ = x;
	y_ = y;
}

void router::read_packet(int iport)
{
	assert(iport < PORTS);

	packet p = port_in[iport].read();

	if ((p.src_x == -1) && (p.src_y == -1))
		return; // empty packet

	route_packet_xy(p);
}

void router::write_packet(int iport)
{
	assert(iport < PORTS);

	if (out_queue_[iport].empty())
	{
		port_out[iport].write(packet()); // write an empty packet
	}
	else
	{
		port_out[iport].write(out_queue_[iport].front());
		out_queue_[iport].pop_front();
	}
}

void router::route_packet_xy(packet p)
{
	if ((p.dest_x == -1) || (p.dest_y == -1))
	{
		printf("router (%d,%d): drop packet with invalid destination"
			" (%d,%d)->(%d,%d)\n",
			x_, y_,
			p.src_x, p.src_y, p.dest_x, p.dest_y);
		return;
	}

	// ignore dest_y for now (1,0 2,0 0,0)
 	if (p.dest_x == x_) // to PE
	{
        out_queue_[PE].push_back(p);
        //modified
        //for router 0,0
        if(x_==0)
        {
            count_R2P_0++;
            if(queue_R0toPO<(size_t)out_queue_[PE].size())
                queue_R0toPO=(size_t)out_queue_[PE].size();
        }
        //for router 1,0
        if(x_==1){
            count_R2P_1++;
            if(queue_R1toP1<(size_t)out_queue_[PE].size())
                queue_R1toP1=(size_t)out_queue_[PE].size();
        }
        if(x_==2){
            count_R2P_2++;
            if(queue_R2toP2<(size_t)out_queue_[PE].size())
                queue_R2toP2=(size_t)out_queue_[PE].size();
        }
		
	}
	else if (p.dest_x < x_) // left to WEST
	{
        //modified
        out_queue_[WEST].push_back(p);
        if(x_==1){
            count_R2R_west_0++;
            if(queue_R1toR0<(size_t)out_queue_[WEST].size())
            queue_R1toR0=(size_t)out_queue_[WEST].size();
        }
        if(x_==2){
            count_R2R_west_1++;
            if(queue_R2toR1<(size_t)out_queue_[WEST].size())
                queue_R2toR1=(size_t)out_queue_[WEST].size();

        }
        

	}
	else // (p.dest_x > x_) right to EAST
	{
        //modified
		out_queue_[EAST].push_back(p);
        
        if(x_==0){
            count_R2R_east_0++;
            if(queue_R0toR1<(size_t)out_queue_[EAST].size())
                queue_R0toR1=(size_t)out_queue_[EAST].size();
        }

        if(x_==1){
            count_R2R_east_1++;
        if(queue_R1toR2<(size_t)out_queue_[EAST].size())
            queue_R1toR2=(size_t)out_queue_[EAST].size();
        }

    }
}
