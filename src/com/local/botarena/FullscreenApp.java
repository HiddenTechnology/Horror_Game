package com.local.botarena;
import android.app.Activity;
import android.app.Application;
import android.os.Bundle;
import android.view.View;
import android.view.WindowManager;
import java.lang.ref.WeakReference;
import java.util.WeakHashMap;
public class FullscreenApp extends Application implements Application.ActivityLifecycleCallbacks{
 private final WeakHashMap<Activity,Keeper> keepers=new WeakHashMap<Activity,Keeper>();
 @Override public void onCreate(){super.onCreate();registerActivityLifecycleCallbacks(this);}
 private Keeper keeper(Activity a){Keeper k=keepers.get(a);if(k==null){k=new Keeper(a);keepers.put(a,k);k.install();}return k;}
 @Override public void onActivityCreated(Activity a,Bundle b){keeper(a);}@Override public void onActivityResumed(Activity a){Keeper k=keeper(a);k.active=true;k.schedule(100);}@Override public void onActivityPaused(Activity a){Keeper k=keepers.get(a);if(k!=null){k.active=false;k.cancel();}}@Override public void onActivityDestroyed(Activity a){Keeper k=keepers.remove(a);if(k!=null)k.release();}@Override public void onActivityStarted(Activity a){}@Override public void onActivityStopped(Activity a){}@Override public void onActivitySaveInstanceState(Activity a,Bundle b){}
 private static class Keeper implements Runnable,View.OnSystemUiVisibilityChangeListener{private final WeakReference<Activity>activity;boolean active=false;Keeper(Activity a){activity=new WeakReference<Activity>(a);}private View decor(){Activity a=activity.get();return a==null?null:a.getWindow().getDecorView();}void install(){View v=decor();if(v!=null)v.setOnSystemUiVisibilityChangeListener(this);}void schedule(long delay){View v=decor();if(v!=null){v.removeCallbacks(this);v.postDelayed(this,delay);}}void cancel(){View v=decor();if(v!=null)v.removeCallbacks(this);}void release(){active=false;cancel();View v=decor();if(v!=null)v.setOnSystemUiVisibilityChangeListener(null);activity.clear();}
 @Override public void run(){Activity a=activity.get();if(!active||a==null||a.isFinishing()||!a.hasWindowFocus())return;a.getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);a.getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE|View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION|View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN|View.SYSTEM_UI_FLAG_HIDE_NAVIGATION|View.SYSTEM_UI_FLAG_FULLSCREEN|View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);}
 @Override public void onSystemUiVisibilityChange(int flags){int hidden=View.SYSTEM_UI_FLAG_HIDE_NAVIGATION|View.SYSTEM_UI_FLAG_FULLSCREEN;if(active&&(flags&hidden)!=hidden)schedule(350);}}
}
