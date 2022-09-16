package de.kai_morich.simple_bluetooth_le_terminal;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;



public class homePage extends AppCompatActivity implements FragmentManager.OnBackStackChangedListener {
    public static Context mContext;
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home_page);

        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        mContext= this;
        getSupportFragmentManager().addOnBackStackChangedListener((FragmentManager.OnBackStackChangedListener) this);

        Button moveButton = (Button) findViewById(R.id.Tosetting);
        moveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(), Settings.class);
                startActivity(intent);
            }
        });

        Button playTwo = (Button) findViewById(R.id.playTwo);
        playTwo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                getSupportFragmentManager().beginTransaction().add(R.id.fragment, new DevicesFragment(), "tt").commit();
            }
        });

    }
    @Override
    public void onBackStackChanged() {
        getSupportActionBar().setDisplayHomeAsUpEnabled(getSupportFragmentManager().getBackStackEntryCount() > 0);

    }
    @Override
    public boolean onSupportNavigateUp() {
        onBackPressed();
        return true;
    }
}

