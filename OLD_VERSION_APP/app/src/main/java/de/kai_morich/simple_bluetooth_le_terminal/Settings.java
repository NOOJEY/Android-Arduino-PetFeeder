package de.kai_morich.simple_bluetooth_le_terminal;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
public class Settings extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.settings);
        Button BTbutton = (Button) findViewById(R.id.toBT);
        BTbutton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent bt = new Intent(getApplicationContext(), MainActivity.class);
                startActivity(bt);
            }
        });

    }
}