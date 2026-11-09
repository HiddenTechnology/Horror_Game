#ifndef WALK_ANIMATION_H
#define WALK_ANIMATION_H
#include "human_audio.h"
#include "zombie.h"
static void drawBone(AssetLibrary&a,const Mesh&m,arena::V3 start,arena::V3 end,float ml,int mat,Color tint){using namespace arena;V3 delta=end-start;float len=length(delta);if(len<.00001f)return;V3 dir=delta*(1/len),axis(-dir.z,0,dir.x);float angle=std::acos(clamp(-dir.y,-1,1))*180/PI;glPushMatrix();glTranslatef(start.x,start.y,start.z);if(length(axis)>.00001f)glRotatef(angle,axis.x,axis.y,axis.z);else if(dir.y>0)glRotatef(180,1,0,0);glScalef(1,len/ml,1);a.draw(m,mat,tint);glPopMatrix();}
static void drawWalker(AssetLibrary&a,const arena::Bot&b,float alpha,const arena::Game&g){using namespace arena;if(b.hp<=0&&b.death<=0)return;human_audio::publish(b,g);int kind=int(b.phase)%4;if(kind==3){drawZombie(a,b,alpha,g);return;}float amount=b.hp>0?clamp(b.speed/1.1f,0,1):0,phase=b.gait,fw[2],up[2],phases[2],hipY=.875f;
for(int k=0;k<2;k++){float ph=std::fmod(phase/(2*PI)+k*.5f,1.f);if(ph<0)ph+=1;phases[k]=ph;float f,h=0;if(ph<.60f)f=.30f*(1-2*ph/.60f);else{float t=(ph-.60f)/.40f;f=-.30f+.60f*t*t*(3-2*t);h=.10f*std::sin(PI*t);}fw[k]=f*amount;up[k]=h*amount;if(ph<.60f||amount<.02f)hipY=std::min(hipY,.08f+std::sqrt(std::max(0.f,.795f*.795f-fw[k]*fw[k])));}float off=hipY-.84f;
Color shirt=kind==0?Color(.16f,.34f,.52f):(kind==1?Color(.48f,.18f,.16f):Color(.22f,.38f,.26f));
Color pants=kind==0?Color(.13f,.15f,.19f):(kind==1?Color(.22f,.23f,.26f):Color(.28f,.25f,.21f));
Color skin=kind==0?Color(.74f,.58f,.46f):(kind==1?Color(.55f,.40f,.30f):Color(.40f,.29f,.22f));
Color shoe=Color(.09f,.09f,.10f);
Color hair=kind==0?Color(.10f,.08f,.07f):(kind==1?Color(.17f,.12f,.08f):Color(.24f,.18f,.11f));
if(b.flash>0){shirt=Color(1,.85f,.75f);skin=Color(1,.85f,.75f);}
glPushMatrix();glTranslatef(b.oldX+(b.x-b.oldX)*alpha,0,b.oldZ+(b.z-b.oldZ)*alpha);glRotatef(b.angle*180/PI,0,1,0);if(b.hp<=0)glRotatef(90*(1-b.death/1.1f),1,0,0);
glPushMatrix();glTranslatef(0,off,0);glRotatef(std::sin(phase)*1.2f*amount,0,1,0);a.draw(a.body[kind],-1,shirt);glPopMatrix();
a.orb(0,.86f+off,0,.26f,.055f,.16f,Color(.07f,.07f,.08f));
a.orb(0,.86f+off,-.155f,.035f,.035f,.012f,Color(.78f,.64f,.22f));
float wl=std::sqrt(b.walkX*b.walkX+b.walkZ*b.walkZ),wx=wl>.01f?b.walkX/wl:0,wz=wl>.01f?b.walkZ/wl:1;
for(int side=-1;side<=1;side+=2){int k=side<0?0:1;float ph=phases[k],f=fw[k],lift=up[k];V3 hip(side*.105f,hipY,0),ankle(side*.105f+wx*f,.08f+lift,wz*f),d=ankle-hip;float dist=length(d);if(dist>.795f){d=d*(.795f/dist);ankle=hip+d;dist=.795f;}V3 dir=d*(1/std::max(dist,.001f)),hint(0,0,1),bend=normalized(hint-dir*dot(hint,dir));float along=dist*.5f,h=std::sqrt(std::max(0.f,.40f*.40f-along*along));V3 knee=hip+dir*along+bend*h;
drawBone(a,a.thigh,hip,knee,.40f,14,pants);drawBone(a,a.calf,knee,ankle,.40f,14,pants);
glPushMatrix();glTranslatef(ankle.x,ankle.y,ankle.z);if(ph>.60f)glRotatef(-8*std::sin((ph-.60f)/.40f*PI)*amount,1,0,0);a.draw(a.foot,1,shoe);glPopMatrix();
float swing=std::sin(phase+(side<0?0.f:PI))*.25f*amount,flex=.23f;if(g.terror&&b.sees&&b.speed>1.3f)swing+=.22f;V3 shoulder(side*.205f,1.32f+off,0),elbow=shoulder+V3(0,-.30f*std::cos(swing),.30f*std::sin(swing)),wrist=elbow+V3(0,-.27f*std::cos(swing+flex),.27f*std::sin(swing+flex));
drawBone(a,a.upperarm,shoulder,elbow,.30f,14,shirt);drawBone(a,a.forearm,elbow,wrist,.27f,5,skin);
glPushMatrix();glTranslatef(wrist.x,wrist.y,wrist.z);glRotatef(-(swing+flex)*180/PI,1,0,0);if(side<0){glScalef(-1,1,1);glFrontFace(GL_CW);}a.draw(a.hand,5,skin);if(side<0)glFrontFace(GL_CCW);glPopMatrix();}
glPushMatrix();glTranslatef(0,1.65f+off,.015f);glScalef(.65f,.70f,.72f);glRotatef(std::sin(g.time*.45f+b.phase)*1.4f+(kind==0?1.f:0),0,0,1);a.draw(a.head[kind],-1,skin);
a.orb(0,.085f,-.01f,.215f,.155f,.225f,hair);
glDisable(GL_LIGHTING);float blink=std::sin(g.time*.65f+b.phase)>.997f?.10f:1.f;
for(int side=-1;side<=1;side+=2){a.orb(side*.057f,.031f,.111f,.016f,.008f*blink,.014f,Color(.92f,.92f,.88f));a.orb(side*.057f,.031f,.124f,.0038f,.0055f*blink,.002f,Color(.03f,.03f,.03f));a.orb(side*.057f-.003f,.034f,.126f,.0015f,.0013f,.001f,Color(.55f,.55f,.52f));}
glEnable(GL_LIGHTING);glPopMatrix();glPopMatrix();}
#endif
