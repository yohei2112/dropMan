/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package net.yohei2112.dropMan;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.os.Bundle;
//admob
import com.google.ads.*;

import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

public class dropMan extends Cocos2dxActivity{
	
	private static AdView adView;
	private static Cocos2dxActivity me = null;
	private final int WC = FrameLayout.LayoutParams.WRAP_CONTENT; 
	private final int MP = FrameLayout.LayoutParams.MATCH_PARENT;
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		adView = new AdView(this, AdSize.BANNER, "");
		FrameLayout.LayoutParams adParams = new FrameLayout.LayoutParams(WC,WC);
		adParams.gravity = (Gravity.BOTTOM|Gravity.CENTER); 
		
		addContentView(adView, adParams);
		AdRequest adRequest = new AdRequest();
		adView.loadAd(adRequest);
		me = this;
	}

	protected void onDestroy()
	{
		if (adView != null)
		{
			adView.destroy();
		}
		super.onDestroy();
		
	}
	
    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// dropMan should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }     

    public static void hideAdView()
    {
    	me.runOnUiThread(new Runnable(){
    		public void run() {
    			if(adView !=null){
    				adView.setVisibility(View.INVISIBLE);
    			}
    		}
    	});
    }

    public static void showAdView()
    {
    	me.runOnUiThread(new Runnable(){
    		public void run() {
    			if(adView !=null){
    				adView.setVisibility(View.VISIBLE);
    			}
    		}
    	});
    }
}
