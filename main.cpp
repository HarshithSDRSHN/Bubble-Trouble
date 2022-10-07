#include <simplecpp>
#include "shooter.h"
#include "bubble.h"

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */

const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);


void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

vector<Bubble> create_bubbles(int nb,double r,double v,Color c)
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0,BUBBLE_START_Y,r*BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX*v,BUBBLE_DEFAULT_VY*v,c));
    bubbles.push_back(Bubble(WINDOW_X/4.0,BUBBLE_START_Y,r*BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX*v,BUBBLE_DEFAULT_VY*v,c));
    if(nb==3){
    bubbles.push_back(Bubble(WINDOW_X*3/4,BUBBLE_START_Y,r*BUBBLE_DEFAULT_RADIUS,
-BUBBLE_DEFAULT_VX*v,BUBBLE_DEFAULT_VY*v,c));
    }
    return bubbles;
}
vector<int> collidebb(vector<Bubble> &bubbles, vector<Bullet> &bullets){
    bool k=false;
    vector <int> v;
    for(int i=0;i<bullets.size();i++){ 
        for(int j=0;j<bubbles.size();j++){
          if(abs(bullets[i].get_center_x()-bubbles[j].get_center_x())<(
             bullets[i].get_width()/2+bubbles[j].get_radius()) &&
             abs(bullets[i].get_center_y()-bubbles[j].get_center_y())<
             (bullets[i].get_height()/2+bubbles[j].get_radius())){
             v.push_back(bubbles[j].get_center_x());
             v.push_back(bubbles[j].get_center_y());
             v.push_back(bubbles[j].get_radius());
             bubbles.erase(bubbles.begin()+j);
             bullets.erase(bullets.begin()+i);
             wait(STEP_TIME/2); 
             k=true; 
             
             if(k) break;
           }
        if(k) break;     
        }
         
     }        
     if(k) return v;
     else { v.push_back(-1); return v;}
}  
bool collidesb(vector<Bubble> &bubbles, Shooter &shooter){
     bool k=false;
     for(int i=0;i<bubbles.size();i++){
        if((abs(bubbles[i].get_center_x()-shooter.get_head_center_x())<
           bubbles[i].get_radius()+shooter.get_head_radius() && 
           abs(bubbles[i].get_center_y()-shooter.get_head_center_y())<
           bubbles[i].get_radius()+shooter.get_head_radius()) || 
           (abs(bubbles[i].get_center_x()-shooter.get_body_center_x())<
           bubbles[i].get_radius()+shooter.get_body_width()/2 &&
           abs(bubbles[i].get_center_y()-shooter.get_body_center_y())<
           bubbles[i].get_radius()+shooter.get_body_height()/2)){
        k=true;   
        break;
        }
     }
     return k;
}
bool splitbubbles(vector<Bubble> &bubbles, vector<Bullet> &bullets){
     vector<int> w; 
     w=collidebb(bubbles,bullets); 
   if(w[0]!=-1 && w[2]>10){
     bubbles.push_back(Bubble(w[0],w[1]
     ,w[2]/2,-BUBBLE_DEFAULT_VX*1.25,BUBBLE_DEFAULT_VY*1.25,
     bubbles[0].get_color()));
     bubbles.push_back(Bubble(w[0],w[1]
     ,w[2]/2,BUBBLE_DEFAULT_VX*1.25,BUBBLE_DEFAULT_VY*1.25,
     bubbles[0].get_color()));
     return true;
   }
   else if(w[0]!=-1)
     return true;
   
   else return false;
}

int main()
{
   initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

   Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
   b1.setColor(COLOR(0, 0, 255));
   int l=1,sc=0;
   string msg_cmd("Cmd: _");
   Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);
   Color c;
   string s("Score: 0"); 
   string win("Congratulations!!");
   string lose("Game Over");
   string h2("/3");
   string lvl("Level: 1/3");
   string exc("!"), spc(" ");
   string t("Time:  "), ffty("/50");
   Text Score(400,BOTTOM_MARGIN, s);
   Text level(250,470,lvl);
   Text Time(LEFT_MARGIN,25,t);
       // Intialize the shooter
   Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);
   
   while(true){
       string h1("Health: 3/3");
       Text health(400,25,h1); 
       string lvlstr("Level ");
       lvlstr = lvlstr + to_string(l) + exc; 
       Text lvldsply(250,250,lvlstr);
       lvldsply.setColor(c=COLOR(0,0,255)); wait(3);
       lvlstr.resize(0); lvldsply.setMessage(lvlstr);
       lvl.erase(6);
       lvl=lvl+spc+to_string(l)+h2;  
       level.setMessage(lvl);
   
    // Initialize the bubbles       
         
          vector<Bubble> bubbles = create_bubbles(2,1,1,c=COLOR(255,105,180));
       if(l==2)
          bubbles = create_bubbles(3,2,1.25,c=COLOR(0,200,255));
       if(l==3)
          bubbles = create_bubbles(3,4,1.56,c=COLOR(100,50,238));
    // Initialize the bullets (empty)
       vector<Bullet> bullets;
       int h=3;
       int ttkn;
       bool q=true;
       std::chrono::duration<double> timetkn(0);
       std::chrono::time_point<std::chrono::system_clock> start,end;
       XEvent event;
    
    // Main game loop
     while (true)
     {
        start = std::chrono::system_clock::now();
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {   
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);
            
            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);             
            else if(c == 'd')
                shooter.move(STEP_TIME, false); 
       
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }
        
        
        // Update the bubbles
        
        move_bubbles(bubbles);
        if(bullets.size()!=0)
         if(splitbubbles(bubbles,bullets)){
           sc=sc+1; 
           s.erase(7);
           s = s+to_string(sc);
           Score.setMessage(s);
         }
        if(bubbles.size()==0){
           l = l+1; wait(1);
           if(l==4){
           Text congrats(250,250,win);
           congrats.setColor(COLOR(0,255,0));
           wait(5);
           }
           break;       
        }
        while(q){
          if(collidesb(bubbles,shooter)){
            h=h-1;
            h1.erase(7);
            h1=h1.substr(0,7)+spc+to_string(h)+h2;
            health.setMessage(h1); 
            q=false;
            shooter.changecolor();
            break;
          }
          break;
        }
        if(!collidesb(bubbles,shooter)){ 
            q=true;
            shooter.originalcolor();
        }
        // Update the bullets
        move_bullets(bullets);
        
        end = std::chrono::system_clock::now();
        timetkn = timetkn + end - start;
        ttkn = timetkn.count();
        t.erase(6);
        t = t+to_string(ttkn)+ffty;
        Time.setMessage(t);
       
        if(h==0 || ttkn==50){
           Text gameover(250,250,lose); 
           gameover.setColor(COLOR(255,0,0));
           wait(5);
           break;
        } 
   
     }
   if(l==4) break;
   if(h==0 || ttkn==50) break;
   }
}