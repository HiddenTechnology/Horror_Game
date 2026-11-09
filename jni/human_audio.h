#ifndef HUMAN_AUDIO_H
#define HUMAN_AUDIO_H
#include <jni.h>
#include <atomic>
#include <cstdint>
#include "game_core.h"
namespace human_audio{static std::atomic<uint32_t>nearest(0xffffffffu);
static void publish(const arena::Bot&b,const arena::Game&g){using namespace arena;if(g.mode!=PLAY||b.hp<=0)return;float dx=b.x-g.x,dz=b.z-g.z,dist=std::sqrt(dx*dx+dz*dz);if(dist>14)return;int dq=int(clamp(dist*12,0,255)),kind=int(b.phase)%4,id=int(b.phase)&63,pq=int((clamp((dx*std::cos(g.yaw)+dz*std::sin(g.yaw))/std::max(.1f,dist),-1,1)+1)*63.5f),step=b.speed>.12f?(int(b.gait/PI)&1):0;uint32_t packed=(uint32_t(dq)<<16)|(uint32_t(kind)<<14)|(uint32_t(step)<<13)|(uint32_t(pq)<<6)|uint32_t(id),old=nearest.load(std::memory_order_relaxed);while(packed<old&&!nearest.compare_exchange_weak(old,packed,std::memory_order_relaxed)){}}
}
extern "C" JNIEXPORT jint JNICALL Java_com_local_botarena_MainActivity_getHumanAudioState(JNIEnv*,jobject){return jint(human_audio::nearest.exchange(0xffffffffu,std::memory_order_relaxed));}
#endif
