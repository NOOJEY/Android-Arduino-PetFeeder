package de.kai_morich.simple_bluetooth_le_terminal;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.view.KeyEvent;
import android.webkit.DownloadListener;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.os.Bundle;

public class CCTV_webView extends AppCompatActivity {
    private String TAG = CCTV_webView.class.getSimpleName();
    private  WebView webView = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_cctv_web_view);

        webView = (WebView) findViewById(R.id.webview);

        webView.setWebViewClient(new WebViewClient());
        webView.setWebChromeClient(new WebChromeClient());
        webView.setDownloadListener((s, s1, s2, s3, l) -> {
        });

        webView.getSettings().setLoadWithOverviewMode(true);
        webView.getSettings().setUseWideViewPort(true);

        webView.getSettings().setSupportZoom(false);
        webView.getSettings().setBuiltInZoomControls(false);

        webView.getSettings().setJavaScriptEnabled(true);
        webView.getSettings().setJavaScriptCanOpenWindowsAutomatically(false);
        webView.getSettings().setSupportMultipleWindows(false);

        webView.getSettings().setDomStorageEnabled(true);

        webView.loadUrl("http://192.168.0.21/");
        //webView.loadUrl("https://naver.com/");

    }
    @Override
    public void onPause(){
        super.onPause();
        webView.onPause();
    }
    @Override
    public void onDestroy(){
        super.onDestroy();
        webView.destroy();
    }

}
