#include <SDL2/SDL.h>
#include <vector>
#include <windows.h>
#include <iostream>
#include <time.h>
#include "c_timer.h"
#include <chrono>
#include <unistd.h>
 
#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4
struct snake{
	int x,y;
	snake * head = nullptr;
	snake * tail = nullptr;
	int px,py;
};

struct food{
	int x,y;
};

class game{
	public:
		int h,l;
		int direction = UP;
		double t1 = get_cur_time();
		double t2;
		int score = 0;
		double sec = 0;
		food * f = new food;
		snake * head = nullptr;
		snake * tail = nullptr;
		bool set = true;
		game(int h,int l){
			this->h = h;
			this->l = l;
			setHead();
		}
		~game(){
			delete f;
			snake * temp = head;
			snake * curr = temp->tail;
			delete temp;
			while(curr !=nullptr){
				temp = curr;
				curr = curr->tail;
				delete temp;
			}
		}
		
		void setHead(){
			head = new snake;
			head->x = l/2;
			head->y = h/2;
			tail = head;
		}
		
		
		setFoodposition(bool *set){
			if(*set == true){
				int x = rand()%l;
				int y = rand()%h;
				while(foodInBody(x,y)){
					x = rand()%l;
					y = rand()%h;
				}
				f->x = x;
				f->y = y;
			}
			*set = false;
		}
		
		
		foodInBody(int x,int y){
			snake * temp= head;
			while(temp->tail != nullptr){
				temp = temp->tail;
				if(x == temp->x && y == temp->y)
					return true;
			}
			return false;
		}
		void addtail(){
			tail->tail = new snake;
			tail->tail->x = tail->px;
			tail->tail->y = tail->py;
			tail->tail->head = tail;
			tail = tail->tail;
		}
		
		void move( SDL_Event event,SDL_bool & done)
		{
			if(SDL_PollEvent(&event)){
				switch(event.type){
					case SDL_KEYUP:
						switch(event.key.keysym.sym){
							
							case SDLK_w:{
									if(direction != DOWN || head->tail == nullptr){
										left();	
										direction = UP;
									}
								}
								break;
							
							case SDLK_s:{
									if(direction != UP || head->tail == nullptr){
										right();
										direction = DOWN;
									}
								}
								break;
							case SDLK_d:{
									if(direction != LEFT || head->tail == nullptr){
										up();
										direction = RIGHT;
									}
								}
								break;
							case SDLK_a:{
									if(direction != RIGHT || head->tail == nullptr){
										down();
										direction = LEFT;
									}
								}
								break;
							case SDLK_RETURN:{
								addtail();
								
							}
							break;	
						}
					

					default:{
						
					}
						break;
				}
				if (event.type == SDL_QUIT) {
					done = SDL_TRUE;
				}
			}
		}
		
		
		void upDatebody(snake *temp){
			while(temp != nullptr){
				;
				temp = temp->tail;
				if(temp != nullptr){
					temp->px = temp->x;
					temp->py = temp->y;
					temp->x = temp->head->px;
					temp->y = temp->head->py;
				}
				
			}
		}
		
		
		bool foodCollision(){
			if(head->x == f->x && head->y == f->y){
				set = true;
				score++;
				return true;
			}
			return false;
		}
		bool wallCollision(){
			if(head->x >= l || head->y >= h || head->x <0 || head->y<0)
				return true;
			return false;
		}
		
		bool selfCollision(){
			snake * temp= head;
			while(temp->tail != nullptr){
				temp = temp->tail;
				if(head->x == temp->x && head->y == temp->y)
					return true;
			}
			return false;
		}
		
		void raparound(){
			head->x = (head->x >= l)? head->x - l :head->x;
			head->y = (head->y >= h)? head->y - h :head->y;
			head->x = (head->x < 0)? l + head->x :head->x;
			head->y = (head->y < 0)? h + head->y :head->y;
		}
		
		void up(){
			
			snake * temp = head;
			temp->px = temp->x;
			temp->py = temp->y;
			temp->y += 1;
			upDatebody(temp);
			raparound();
			
		}
		void right(){
			snake * temp = head;
			temp->px = temp->x;
			temp->py = temp->y;
			temp->x += 1;
			upDatebody(temp);
			raparound();
			
		}
		
		void down(){
			snake * temp = head;
			temp->px = temp->x;
			temp->py = temp->y;
			temp->y -= 1;
			upDatebody(temp);
			raparound();
			
		}
		
		void left(){
			snake * temp = head;
			temp->px = temp->x;
			temp->py = temp->y;
			temp->x -= 1;
			upDatebody(temp);
			raparound();
			
		}
		
		void idle (int dir){
			t2 = get_cur_time();
			sec = t2 - t1;
			//std::cout<<sec<<std::endl;
			if(sec  >= 0.25){
				
				if(dir == UP){
					left();	
				}				
				if(dir == DOWN){
					right();
				}
				if(dir == RIGHT){
					up();
				}
				if(dir == LEFT){
					down();
				}
				t1 = get_cur_time();
			}
			
		}
		
		void print(){
			snake * temp = head;
			while(temp != nullptr){
				std::cout<<" x: "<<temp->x<<" y: "<<temp->y;
				std::cout<<" px: "<<temp->px<<" py: "<<temp->py;
				if(temp->head != nullptr){
					std::cout<<" haedx: "<<temp->head->x<<" heady: "<<temp->head->y;
				}
				if(temp->tail != nullptr){
					std::cout<<" tailx: "<<temp->tail->x<<" taily: "<<temp->tail->y;
				}
				std::cout<<std::endl;
				
				
				temp = temp->tail;
			}
			std::cout<<" tailx: "<<tail->x<<" taily: "<<tail->y<<" px: "<<tail->px<<" py: "<<tail->py<<std::endl;
			std::cout<<std::endl;
		}
		
		bool isBody(int x,int y){
			snake *temp = head;
			while(temp != nullptr){
				if(temp->x == x && temp->y == y)
					return true;
				temp = temp->tail;
			}
			return false;
		}
		
		void draw(SDL_Renderer* renderer){
			int i,j;
			int r=0,g=0,b=0;
				for(i =0; i < h ;i++){
					for(j = 0 ; j < l ; j++){
						r=0,g=0,b=0;
						if(f->x == j && f->y ==i){
							r=255;g=255;b =0;
						}
						if(head->x ==j && head->y ==i){
							r=255;g=0;b =0;
						}	
						else if(isBody(j,i)){
							r=255;g=255;b =255;
						}
						
						drawRect(renderer,25,25,100+(i*25),100+(j*25),r,g,b,255);
						
						
					}
				}
				drawLine(renderer,100,100,100+(i*25),100,255,255,255,255);
				drawLine(renderer,100,100,100,100+(h*25),255,255,255,255);
				drawLine(renderer,100+(i*25),100,100+(i*25),100+(25*h),255,255,255,255);
				drawLine(renderer,100,100+(j*25),100+(25*l),100+(j*25),255,255,255,255);
		}
		
		void drawRect(SDL_Renderer* renderer,int w , int h , int x,int y ,int r ,int g,int b,int a)const{
			SDL_Rect rect;
			
			rect.h = w;
			rect.w = h;
			rect.x = x;
			rect.y = y;
			
			SDL_SetRenderDrawColor(renderer, r,g,b,a);
			SDL_RenderFillRect(renderer,&rect);
			
		}
		
		void drawLine(SDL_Renderer* renderer,int x1,int y1,int x2,int y2 ,int r ,int g,int b,int a)const{
		
			SDL_SetRenderDrawColor(renderer, r,g,b,a);
			SDL_RenderDrawLine(renderer,x1,y1,x2,y2);
		}
	
};

int main(int argc, char* argv[]){
	srand(time(NULL));
	DWORD screenWidth = GetSystemMetrics(SM_CXSCREEN);
	DWORD screenHeight = GetSystemMetrics(SM_CYSCREEN);
	game * g = new game(18,18);
	
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
		

		 SDL_Event event;
        if (SDL_CreateWindowAndRenderer(800,600, 0, &window, &renderer) == 0) {
            SDL_bool done = SDL_FALSE;
			
            while (!g->selfCollision() &&! g->wallCollision() && !done) {
               

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
				if (g->foodCollision())
					g->addtail();
				
				g->setFoodposition(&g->set);
                g->draw(renderer);
				g->move(event,done);
				g->idle(g->direction);
				//g->print();
				std::cout<<"score "<<g->score<<std::endl;
                SDL_RenderPresent(renderer);
				
            }
			delete g;
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					done = SDL_TRUE;
				}
			}
        }

        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }
    SDL_Quit();
    return 0;
}

