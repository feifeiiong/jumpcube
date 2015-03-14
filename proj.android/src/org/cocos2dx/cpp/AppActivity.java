/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
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
package org.cocos2dx.cpp;

import org.apache.http.Header;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.EditText;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;
import com.umeng.social.CCUMSocialController;

public class AppActivity extends Cocos2dxActivity {

	public static Handler handler;
	public static final int BANNER_OPEN = 0;
	public static final int BANNER_CLOSE = 1;
	private static boolean isBannerOpen = true;

	public static void showQuitDialog() {
		Message msg = handler.obtainMessage();
		msg.what = 2;
		handler.sendMessage(msg);

	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		return super.onKeyDown(keyCode, event);
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		CCUMSocialController.onActivityResult(requestCode, resultCode, data);
		super.onActivityResult(requestCode, resultCode, data);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		// requestForADs();
		CCUMSocialController.initSocialSDK(this, "com.flappyrec.android.share");

		handler = new Handler() {

			@Override
			public void handleMessage(Message msg) {
				switch (msg.what) {
				case BANNER_OPEN:
					break;
				case 2:
					onQuitDialogShow();
					break;
				}
			}

		};

	}

	private void requestForADs() {
		AsyncHttpClient client = new AsyncHttpClient();
		client.post("http://jumpcube.sinaapp.com/req_ad",
				new AsyncHttpResponseHandler() {

					@Override
					public void onSuccess(int statusCode, Header[] headers,
							byte[] responseBody) {

						try {
							String json = new String(responseBody);
							Log.v("success", json);
							if (json.substring(0, 1).equals("[")) {

								JSONArray jarray = new JSONArray(json);
								Log.v("isbanner", isBannerOpen + "array");
							} else {
								JSONObject jo = new JSONObject(json);
								isBannerOpen = jo.getBoolean("isBannerOpen");
							}
						} catch (JSONException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
					}

				});
		// client.post("http://jumpcube.sinaapp.com/req_ad/", new
		// AsyncHttpResponseHandler() {
		//
		// @Override
		// public void onSuccess(int statusCode, Header[] headers,
		// byte[] responseBody) {
		// Log.v("success", "success");
		// try {
		// String json = new String(responseBody);
		// if (json.substring(0, 1).equals("[")) {
		//
		// JSONArray jarray = new JSONArray(json);
		// Log.v("isbanner", isBannerOpen+"array");
		// } else {
		// JSONObject jo = new JSONObject(json);
		// isBannerOpen = jo.getBoolean("isBannerOpen");
		// Log.v("isbanner", isBannerOpen+"");
		//
		// }
		// } catch (JSONException e) {
		// // TODO Auto-generated catch block
		// e.printStackTrace();
		// }
		// }
		//
		// });
	}

	public static void onQuitDialogShow() {
		AlertDialog.Builder builder = new Builder(getContext());
		AlertDialog dialog = builder.create();
		dialog.setTitle("确定退出游戏吗？");
		dialog.setButton(AlertDialog.BUTTON_POSITIVE, "确定",
				new DialogInterface.OnClickListener() {

					@Override
					public void onClick(DialogInterface arg0, int arg1) {
						System.exit(0);
					}
				});
		dialog.setButton(AlertDialog.BUTTON_NEGATIVE, "取消",
				new DialogInterface.OnClickListener() {

					@Override
					public void onClick(DialogInterface arg0, int arg1) {
						arg0.dismiss();
					}
				});
		dialog.show();
	}

	@Override
	protected void onDestroy() {

		super.onDestroy();
	}

	public void onClickHideShow() {

	}

}
