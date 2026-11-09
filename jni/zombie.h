#ifndef ZOMBIE_H
#define ZOMBIE_H
#include "game_core.h"
static void zLimb(AssetLibrary&a,float x0,float y0,float z0,float x1,float y1,float z1,float r,Color c){
 using namespace arena;
 int n=7;
 for(int i=0;i<n;i++){
  float t=(i+0.5f)/n;
  float x=x0+(x1-x0)*t,y=y0+(y1-y0)*t,z=z0+(z1-z0)*t;
  float w=1.2f-0.4f*std::fabs(t-0.5f)*2;
  a.orb(x,y,z,r*w,r*w,r*w,c);
 }
}
static void drawZombie(AssetLibrary&a,const arena::Bot&b,float alpha,const arena::Game&g){
 using namespace arena;
 if(b.hp<=0&&b.death<=0)return;
 float px=b.oldX+(b.x-b.oldX)*alpha,pz=b.oldZ+(b.z-b.oldZ)*alpha;
 float t=b.gait,amt=b.hp>0?clamp(b.speed/1.1f,0,1):0;
 float seed=b.seed;
 Color skin(0.47f,0.56f,0.36f),skinD(0.37f,0.46f,0.28f),cloth(0.16f,0.18f,0.15f),clothD(0.11f,0.12f,0.10f),blood(0.38f,0.06f,0.05f),eye(0.95f,0.85f,0.25f);
 glPushMatrix();
 glTranslatef(px,0,pz);
 glRotatef(b.angle*180/PI,0,1,0);
 if(b.hp<=0)glRotatef(90*(1-b.death/1.1f),1,0,0);
 float lurch=std::sin(t*0.5f+seed)*5;
 glRotatef(14+lurch,1,0,0);
 glRotatef(std::sin(t+seed)*6,0,0,1);
 glTranslatef(0,std::fabs(std::sin(t))*0.05f*amt,0);
 a.orb(0,1.24f,0,0.185f,0.24f,0.125f,cloth);
 a.orb(0,1.10f,0.01f,0.16f,0.14f,0.105f,clothD);
 a.orb(0.05f,1.27f,0.08f,0.06f,0.07f,0.04f,skin);
 a.orb(-0.06f,1.17f,0.07f,0.05f,0.06f,0.04f,skin);
 a.orb(0.06f,1.29f,0.09f,0.045f,0.055f,0.03f,blood);
 a.orb(-0.05f,1.19f,0.08f,0.04f,0.05f,0.03f,blood);
 float hy=1.50f,hx=std::sin(t*0.9f+seed)*0.025f;
 a.orb(hx,hy,0.015f,0.115f,0.135f,0.12f,skin);
 a.orb(hx,hy-0.10f,0.055f,0.08f,0.04f,0.07f,skinD);
 a.orb(hx-0.042f,hy+0.025f,0.095f,0.024f,0.017f,0.017f,eye);
 a.orb(hx+0.042f,hy+0.025f,0.095f,0.024f,0.017f,0.017f,eye);
 a.orb(hx+0.06f,hy+0.04f,0.05f,0.028f,0.035f,0.025f,blood);
 for(int s=-1;s<=1;s+=2){
  float reach=0.30f+std::sin(t+s+seed)*0.04f*amt;
  V3 sh(s*0.20f,1.35f,-0.02f),el(s*0.23f,1.15f,0.15f),ha(s*0.20f,1.07f,0.38f+reach*0.3f);
  zLimb(a,sh.x,sh.y,sh.z,el.x,el.y,el.z,0.07f,skin);
  zLimb(a,el.x,el.y,el.z,ha.x,ha.y,ha.z,0.055f,skin);
  a.orb(ha.x,ha.y,ha.z,0.048f,0.065f,0.055f,skinD);
  a.orb(ha.x-0.028f,ha.y-0.035f,ha.z+0.05f,0.016f,0.016f,0.045f,skinD);
  a.orb(ha.x+0.028f,ha.y-0.035f,ha.z+0.05f,0.016f,0.016f,0.045f,skinD);
 }
 for(int s=-1;s<=1;s+=2){
  float ph=std::fmod(t/(2*PI)+(s<0?0:0.5f),1.f);if(ph<0)ph+=1;
  float sw=std::sin(ph*2*PI)*0.32f*amt;
  float lift=std::max(0.f,std::sin(ph*2*PI))*0.12f*amt;
  V3 hip(s*0.105f,0.94f,0),knee(s*0.12f+sw*0.18f,0.50f+lift*0.45f,sw*0.38f),foot(s*0.105f+sw*0.38f,0.06f+lift,sw*0.65f);
  zLimb(a,hip.x,hip.y,hip.z,knee.x,knee.y,knee.z,0.082f,cloth);
  zLimb(a,knee.x,knee.y,knee.z,foot.x,foot.y,foot.z,0.062f,skinD);
  a.orb(foot.x,foot.y,foot.z,0.055f,0.045f,0.095f,Color(0.10f,0.10f,0.11f));
 }
 glPopMatrix();
}
#endif
