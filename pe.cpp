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

int count_Output =0;


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
	if (r < 20)
		fire_PI();

	// fire PO if the incoming packet is valid
	if ((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1))
		fire_PO();
}

void PE_IO::fire_PI()
{
    //****modified****

    /*count_PI++;
	int P1_x = 1, P1_y = 0;
	*/
	packet p0(x_, y_, 0, 0, rand() % 100, rand() % 100); //send to 1
	packet p1(x_, y_, 0, 1, rand() % 100, rand() % 100); //send to 3
	packet p2(x_, y_, 1, 2, rand() % 100, rand() % 100); //send to 2
	packet p3(x_, y_, 2, 1, rand() % 100, rand() % 100); //send to 4
	packet p4(x_, y_, 0, 0, rand() % 100, rand() % 100); //send to 1
	packet p5(x_, y_, 0, 1, rand() % 100, rand() % 100); //send to 3
	packet p6(x_, y_, 1, 2, rand() % 100, rand() % 100); //send to 2
	packet p7(x_, y_, 2, 1, rand() % 100, rand() % 100); //send to 4

	//packet p(x_, y_, P1_x, P1_y, rand()%100);

	printf("PI: sending 8 packet %.4f + %.4f i, %.4f + %.4f i, %.4f + %.4f i,%.4f + %.4f i, %.4f + %.4f i, %.4f + %.4f i, %.4f + %.4f i,%.4f + %.4f i to queue"
		, p0.rl,p0.im,p1.rl,p1.im,p2.rl,p2.im,p3.rl,p3.im,p4.rl,p4.im,p4.rl,p5.rl,p5.im,p6.rl,p6.im,p7.rl,p7.im);
	out_queue_.push_back(p0);
	out_queue_.push_back(p1);
	out_queue_.push_back(p2);
	out_queue_.push_back(p3);
	out_queue_.push_back(p4);
	out_queue_.push_back(p5);
	out_queue_.push_back(p6);
	out_queue_.push_back(p7);
	/*printf("PI: send %d to (%d,%d)\n",
		p.token, p.dest_x, p.dest_y);*/
    //****modified****
    //count_P2R_0++;

	
    
    //***modified queue size***
    /*if(queue_PO<(size_t)out_queue_.size()){
        queue_PO=(size_t)out_queue_.size();
    }*/
    
}

void PE_IO::fire_PO()
{
	assert((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1));
    //****modified
    //count_PO++;
	//count++;
	//result = result << packet_in_.rl;
    count_Output++;
    if(count_Output == 9){
        count_Output = 1;
    }
	printf("PO: receive %.4f %.4f i from (%d,%d)", packet_in_.rl, packet_in_.im, packet_in_.src_x, packet_in_.src_y);

	//printf("PO: receive %d from (%d,%d)\n",
		//packet_in_.token, packet_in_.src_x,	packet_in_.src_y);
}

void PE_inc::execute()
{
	// fire the actor if the incoming packet is valid
	if ((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1))
		fire();
}

double PE_inc::addRL(double rl1, double rl2)
{
	return rl1 + rl2;
}
double PE_inc::addIm(double im1, double im2)
{
	return im1 + im2;
}
double PE_inc::minusRL(double rl1, double rl2)
{
	return rl1 - rl2;
}
double PE_inc::minusIm(double im1, double im2)
{
	return im1 - im2;
}



double PE_inc::add_w_Rl(double rl1, double rl2, double im2){
	double wRl = 0.707;
	double wIm = 0.707;
	return rl1 + rl2*wRl - im2*wIm;

}
double PE_inc::add_w_Im(double im1, double rl2, double im2)
{
	double wRl = 0.707;
	double wIm = 0.707;
	return im1 + rl2*wIm + im2*wRl;

}
double PE_inc::minus_w_Rl(double rl1, double rl2, double im2){
	double wRl = 0.707;
	double wIm = 0.707;
	return rl1 - rl2*wRl + im2*wIm;

}
double PE_inc::minus_w_Im(double im1, double rl2, double im2)
{
	double wRl = 0.707;
	double wIm = 0.707;
	return im1 - rl2*wIm - im2*wRl;

}
double PE_inc::add_w2_Rl(double rl1, double rl2, double im2){
	double wRl = 0.0;
	double wIm = 1.0;
	return rl1 + rl2*wRl - im2*wIm;

}
double PE_inc::add_w2_Im(double im1, double rl2, double im2)
{
	double wRl = 0.0;
	double wIm = 1.0;
	return im1 + rl2*wIm + im2*wRl;
}
double PE_inc::minus_w2_Rl(double rl1, double rl2, double im2){
	double wRl = 0.0;
	double wIm = 1.0;
	return rl1 - rl2*wRl + im2*wIm;

}
double PE_inc::minus_w2_Im(double im1, double rl2, double im2)
{
	double wRl = 0.0;
	double wIm = 1.0;
	return im1 - rl2*wIm - im2*wRl;

}
double PE_inc::add_w3_Rl(double rl1, double rl2, double im2){
	double wRl = -0.707;
	double wIm = 0.707;
	return rl1 + rl2*wRl - im2*wIm;

}
double PE_inc::add_w3_Im(double im1, double rl2, double im2)
{
	double wRl = -0.707;
	double wIm = 0.707;
	return im1 + rl2*wIm + im2*wRl;
}
double PE_inc::minus_w3_Rl(double rl1, double rl2, double im2){
	double wRl = -0.707;
	double wIm = 0.707;
	return rl1 - rl2*wRl + im2*wIm;

}
double PE_inc::minus_w3_Im(double im1, double rl2, double im2)
{
	double wRl = -0.707;
	double wIm = 0.707;
	return im1 - rl2*wIm - im2*wRl;
}


void PE_inc::fire()
{

	assert((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1));

	//********************proj 2*****************************
	if (!alreadyHaveNumberInPe){
		alreadyHaveNumberInPe = true;
		lastNumberRl = packet_in_.rl;
		lastNumberIm = packet_in_.im;
		source_x = packet_in_.src_x;
		source_y = packet_in_.src_y;
	}
	else{
		//not desired push to end
		if (packet_in_.src_x == source_x && packet_in_.src_y == source_y && source_x != 1 && source_y != 1){
			out_queue_.push_back(packet_in_);
		}
		else{
			//process 1 , 9
			if (x_ == 0 && y_ == 0)
			{
				//process 1 match
				if (source_x == 1 && source_y == 1 && packet_in_.src_x ==1 && packet_in_.src_y == 1){
					double new_add_rl = addRL(lastNumberRl, packet_in_.rl);
					double new_add_im = addIm(lastNumberIm, packet_in_.im);
					double new_minus_rl = minusRL(lastNumberRl, packet_in_.rl);
					double new_minus_im = minusIm(lastNumberIm, packet_in_.im);
					packet p1(x_, y_, 0, 1,new_add_rl,new_add_im);
					packet p2(x_, y_, 0, 2, new_minus_rl, new_minus_im);
					//send to 5(0,1)
					//send to 6(0,2)
					out_queue_.push_back(p1);
					out_queue_.push_back(p2);
				}
				//process 9 match 
				else if ((source_x ==0 && source_y == 1 && packet_in_.src_x == 2 && packet_in_.src_y == 1)||(source_x == 2 && source_y == 1 && packet_in_.src_x == 0 && packet_in_.src_y ==1))
				{
					double new_add_rl = addRL(lastNumberRl, packet_in_.rl);
					double new_add_im = addIm(lastNumberIm, packet_in_.im);
					double new_minus_rl = minusRL(lastNumberRl, packet_in_.rl);
					double new_minus_im = minusIm(lastNumberIm, packet_in_.im);
					packet p1(x_, y_, 1, 1, new_add_rl, new_add_im);
					packet p2(x_, y_, 1, 1, new_minus_rl, new_minus_im);
					out_queue_.push_back(p1);
					out_queue_.push_back(p2);
					//send to pio(1,1)
					//send to pio(1,1)
					
				}
				else{
					out_queue_.push_back(packet_in_);
				}
			}
			//process 8 w2
			else if (x_ == 1 && y_ == 0)
			{
				double new_add_rl = add_w2_Rl(lastNumberRl, packet_in_.rl,packet_in_.im);
				double new_add_im = add_w2_Im(lastNumberIm, packet_in_.im,packet_in_.im);
				double new_minus_rl = minus_w2_Rl(lastNumberRl, packet_in_.rl,packet_in_.im);
				double new_minus_im = minus_w2_Im(lastNumberIm, packet_in_.im,packet_in_.im);
				packet p1(x_, y_, 0, 2, new_add_rl, new_add_im);
				packet p2(x_, y_, 2, 2, new_minus_rl, new_minus_im);
				out_queue_.push_back(p1);
				out_queue_.push_back(p2);
				//send to 11(0,2)
				//send to 12(2,2)
			}
			//process 10 w
			else if (x_ == 2 && y_ == 0)
			{
				double new_add_rl = add_w_Rl(lastNumberRl, packet_in_.rl, packet_in_.im);
				double new_add_im = add_w_Im(lastNumberIm, packet_in_.im, packet_in_.im);
				double new_minus_rl = minus_w_Rl(lastNumberRl, packet_in_.rl, packet_in_.im);
				double new_minus_im = minus_w_Im(lastNumberIm, packet_in_.im, packet_in_.im);
				packet p1(x_, y_, 1, 1, new_add_rl, new_add_im);
				packet p2(x_, y_, 1, 1, new_minus_rl, new_minus_im);
				out_queue_.push_back(p1);
				out_queue_.push_back(p2);
				//send to pio (1,1)
				//send to pio (1,1)
			}
			//process 3 5
			else if (x_ == 1 & y_ == 0)
			{
				//process 3 match
				if (source_x == 1 && source_y == 1 && packet_in_.src_x == 1 && packet_in_.src_y == 1){
					double new_add_rl = addRL(lastNumberRl, packet_in_.rl);
					double new_add_im = addIm(lastNumberIm, packet_in_.im);
					double new_minus_rl = minusRL(lastNumberRl, packet_in_.rl);
					double new_minus_im = minusIm(lastNumberIm, packet_in_.im);
					packet p1(x_, y_, 2, 1, new_add_rl, new_add_im);
					packet p2(x_, y_, 1, 0, new_minus_rl, new_minus_im);
					out_queue_.push_back(p1);
					out_queue_.push_back(p2);
					//send to 7(2,1)
					//send to 8(1,0)
				}
				//process 5 match 
				else if ((source_x == 0 && source_y == 0 && packet_in_.src_x == 1 && packet_in_.src_y == 2) || (source_x == 1 && source_y == 2 && packet_in_.src_x == 0 && packet_in_.src_y == 0))
				{
					double new_add_rl = addRL(lastNumberRl, packet_in_.rl);
					double new_add_im = addIm(lastNumberIm, packet_in_.im);
					double new_minus_rl = minusRL(lastNumberRl, packet_in_.rl);
					double new_minus_im = minusIm(lastNumberIm, packet_in_.im);
					packet p1(x_, y_, 0, 0, new_add_rl, new_add_im);
					packet p2(x_, y_, 2, 2, new_minus_rl, new_minus_im);
					out_queue_.push_back(p1);
					out_queue_.push_back(p2);
					//send to 9(0,0)
					//send to 12(2,2)
				}
				else{
					out_queue_.push_back(packet_in_);
				}
			}
			//process 4, 7
			else if (x_ == 2 && y_ == 1)
			{
				//process 4 match
				if (source_x == 1 && source_y == 1 && packet_in_.src_x == 1 && packet_in_.src_y == 1){
					double new_add_rl = addRL(lastNumberRl, packet_in_.rl);
					double new_add_im = addIm(lastNumberIm, packet_in_.im);
					double new_minus_rl = minusRL(lastNumberRl, packet_in_.rl);
					double new_minus_im = minusIm(lastNumberIm, packet_in_.im);
					packet p1(x_, y_, 2, 1, new_add_rl, new_add_im);
					packet p2(x_, y_, 1, 0, new_minus_rl, new_minus_im);
					out_queue_.push_back(p1);
					out_queue_.push_back(p2);
					//send to 7(2,1)
					//send to 8(1,0)
				}
				//process 7 match 
				else if ((source_x == 0 && source_y == 1 && packet_in_.src_x == 2 && packet_in_.src_y == 1) || (source_x == 2 && source_y == 1 && packet_in_.src_x == 0 && packet_in_.src_y == 1))
				{
					double new_add_rl = addRL(lastNumberRl, packet_in_.rl);
					double new_add_im = addIm(lastNumberIm, packet_in_.im);
					double new_minus_rl = minusRL(lastNumberRl, packet_in_.rl);
					double new_minus_im = minusIm(lastNumberIm, packet_in_.im);
					packet p1(x_, y_, 0, 0, new_add_rl, new_add_im);
					packet p2(x_, y_, 0, 2, new_minus_rl, new_minus_im);
					out_queue_.push_back(p1);
					out_queue_.push_back(p2);
					//send to 9(0,0)
					//send to 11(0,2)
				}
				else{
					out_queue_.push_back(packet_in_);
				}

			}
			//process 6, 11  w2
			else if (x_ == 0 && y_ == 2)
			{
				//process 6 match
				if ((source_x == 0 && source_y == 0 && packet_in_.src_x == 1 && packet_in_.src_y == 2) || (source_x == 1 && source_y == 2 && packet_in_.src_x == 0 && packet_in_.src_y == 0)){
					double new_add_rl = add_w2_Rl(lastNumberRl, packet_in_.rl, packet_in_.im);
					double new_add_im = add_w2_Im(lastNumberIm, packet_in_.im, packet_in_.im);
					double new_minus_rl = minus_w2_Rl(lastNumberRl, packet_in_.rl, packet_in_.im);
					double new_minus_im = minus_w2_Im(lastNumberIm, packet_in_.im, packet_in_.im);
					packet p1(x_, y_, 2, 0, new_add_rl, new_add_im);
					packet p2(x_, y_, 2, 2, new_minus_rl, new_minus_im);
					out_queue_.push_back(p1);
					out_queue_.push_back(p2);
					//send to 10(2,0)
					//send to 12(2,2)
				}
				//process 11 match 
				else if ((source_x == 0 && source_y == 1 && packet_in_.src_x == 0 && packet_in_.src_y == 2) || (source_x == 0 && source_y == 2 && packet_in_.src_x == 0 && packet_in_.src_y == 1))
				{
					double new_add_rl = add_w2_Rl(lastNumberRl, packet_in_.rl, packet_in_.im);
					double new_add_im = add_w2_Im(lastNumberIm, packet_in_.im, packet_in_.im);
					double new_minus_rl = minus_w2_Rl(lastNumberRl, packet_in_.rl, packet_in_.im);
					double new_minus_im = minus_w2_Im(lastNumberIm, packet_in_.im, packet_in_.im);
					packet p1(x_, y_, 1, 1, new_add_rl, new_add_im);
					packet p2(x_, y_, 1, 1, new_minus_rl, new_minus_im);
					out_queue_.push_back(p1);
					out_queue_.push_back(p2);
					//send to pio(1,1)
					//send to pio(1,1)
				}
				else{
					out_queue_.push_back(packet_in_);
				}
			}
			// process 2
			else if (x_ == 1 && y_ == 2)
			{
				double new_add_rl = addRL(lastNumberRl, packet_in_.rl);
				double new_add_im = addIm(lastNumberIm, packet_in_.im);
				double new_minus_rl = minusRL(lastNumberRl, packet_in_.rl);
				double new_minus_im = minusIm(lastNumberIm, packet_in_.im);
				packet p1(x_, y_, 0, 1, new_add_rl, new_add_im);
				packet p2(x_, y_, 0, 2, new_minus_rl, new_minus_im);
				out_queue_.push_back(p1);
				out_queue_.push_back(p2);
				//send to 5(0,1)
				//sent to 6(0,2)
			}
			// process 12 w3
			else if (x_ == 2 && y_ == 2)
			{
				double new_add_rl = add_w3_Rl(lastNumberRl, packet_in_.rl, packet_in_.im);
				double new_add_im = add_w3_Im(lastNumberIm, packet_in_.im, packet_in_.im);
				double new_minus_rl = minus_w3_Rl(lastNumberRl, packet_in_.rl, packet_in_.im);
				double new_minus_im = minus_w3_Im(lastNumberIm, packet_in_.im, packet_in_.im);
				packet p1(x_, y_, 1, 1, new_add_rl, new_add_im);
				packet p2(x_, y_, 1, 1, new_minus_rl, new_minus_im);
				out_queue_.push_back(p1);
				out_queue_.push_back(p2);
				//send to pio (1,1)
				//send to pio (1,1)
			}
		}
	}

	//*******************end proj2***************************
    //***********modified************

	/*int PO_x = 0, PO_y = 0;
    
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
	/*
    if(x_==1&&queue_P1<(size_t)out_queue_.size()){
        queue_P1=(size_t)out_queue_.size();
    }
    if(x_==2&&queue_P2<(size_t)out_queue_.size()){
        queue_P2=(size_t)out_queue_.size();
    }*/
}
