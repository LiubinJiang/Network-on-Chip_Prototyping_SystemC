#include "pe.h"

int count_PO=0;
int count_PI=0;
int count_P1=0;
int count_P2=0;

int count_P2R_0=0;
int count_P2R_1=0;
int count_P2R_2=0;

size_t queue_PO=0;
size_t queue_P1=0;
size_t queue_P2=0;


void PE_base::set_xy(int x, int y)
{
	assert((x_ == -1) && (y_ == -1)); // set once only
	assert((x != -1) && (y != -1)); // must use a legal location

	x_ = x;
	y_ = y;
}

void PE_base::read_input()
{
	packet_in_ = data_in.read();
}

void PE_base::write_output()
{
	if (out_queue_.empty())
	{
		data_out.write(packet());
	}
	else
	{
		data_out.write(out_queue_.front());
		out_queue_.pop_front();
	}
}

void PE_IO::execute()
{
	// decide if we are going to fire PI
	int r = rand()%100;
	if (r < 80)
		fire_PI();

	// fire PO if the incoming packet is valid
	if ((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1))
		fire_PO();
}

void PE_IO::fire_PI()
{
    //****modified****
    count_PI++;
	int P1_x = 1, P1_y = 0;

	packet p(x_, y_, P1_x, P1_y, rand()%100);

	printf("PI: send %d to (%d,%d)\n",
		p.token, p.dest_x, p.dest_y);
    //****modified****
    count_P2R_0++;

	out_queue_.push_back(p);
    
    //***modified queue size***
    if(queue_PO<(size_t)out_queue_.size()){
        queue_PO=(size_t)out_queue_.size();
    }
    
}

void PE_IO::fire_PO()
{
	assert((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1));
    //****modified
    count_PO++;

	printf("PO: receive %d from (%d,%d)\n",
		packet_in_.token, packet_in_.src_x,	packet_in_.src_y);
}

void PE_inc::execute()
{
	// fire the actor if the incoming packet is valid
	if ((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1))
		fire();
}

void PE_inc::fire()
{
	assert((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1));
    
    //***********modified************

	int PO_x = 0, PO_y = 0;
    
    if(x_==1){
        PO_x=2;
        count_P1++;
        count_P2R_1++;
    }else if(x_==2){
        PO_x=0;
        count_P2++;
        count_P2R_2++;
    }
	packet p(x_, y_, PO_x, PO_y, packet_in_.token+1);

	printf("inc(%d,%d): receive %d from (%d,%d), send %d to (%d,%d)\n",
		x_, y_,
		packet_in_.token, packet_in_.src_x,	packet_in_.src_y,
		p.token, p.dest_x, p.dest_y);
    //*****modified****
    //count_P1++;
    //count_P2R_1++;

	out_queue_.push_back(p);
    if(x_==1&&queue_P1<(size_t)out_queue_.size()){
        queue_P1=(size_t)out_queue_.size();
    }
    if(x_==2&&queue_P2<(size_t)out_queue_.size()){
        queue_P2=(size_t)out_queue_.size();
    }
}
