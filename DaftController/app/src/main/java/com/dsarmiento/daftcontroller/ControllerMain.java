package com.dsarmiento.daftcontroller;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.ToggleButton;

import java.util.LinkedList;

public class ControllerMain extends AppCompatActivity {
    LinkedList<ToggleButton> toggleButtons;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_controller_main);
        toggleButtons = new LinkedList<>();

        final ToggleButton roboCop = (ToggleButton) findViewById(R.id.roboCop);
        final ToggleButton heartBeat = (ToggleButton) findViewById(R.id.heartBeat);
        final ToggleButton heartBlink = (ToggleButton) findViewById(R.id.heartBlink);
        final ToggleButton blinkingEyes = (ToggleButton) findViewById(R.id.blinkingEyes);
        final ToggleButton whiteNoise = (ToggleButton) findViewById(R.id.whiteNoise);
        final ToggleButton marqueeText = (ToggleButton) findViewById(R.id.marqueeText);
        final ToggleButton staticText = (ToggleButton) findViewById(R.id.staticText);
        final ToggleButton pacMan = (ToggleButton) findViewById(R.id.pacMan);
        final ToggleButton allOn = (ToggleButton) findViewById(R.id.allOn);
        final ToggleButton allOff = (ToggleButton) findViewById(R.id.allOff);

        toggleButtons.add(roboCop);
        toggleButtons.add(heartBeat);
        toggleButtons.add(heartBlink);
        toggleButtons.add(blinkingEyes);
        toggleButtons.add(whiteNoise);
        toggleButtons.add(marqueeText);
        toggleButtons.add(staticText);
        toggleButtons.add(pacMan);
        toggleButtons.add(allOn);
        toggleButtons.add(allOff);

        roboCop.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    turnOff(roboCop);
                }
            }
        });

        heartBeat.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    turnOff(heartBeat);
                }
            }
        });

        heartBlink.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    turnOff(heartBlink);
                }
            }
        });

        blinkingEyes.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    turnOff(blinkingEyes);
                }
            }
        });

        whiteNoise.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    turnOff(whiteNoise);
                }
            }
        });

        marqueeText.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    turnOff(marqueeText);
                }
            }
        });

        staticText.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    turnOff(staticText);
                }
            }
        });

        pacMan.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    turnOff(pacMan);
                }
            }
        });

        allOn.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    turnOff(allOn);
                }
            }
        });

        allOff.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    turnOff(allOff);
                }
            }
        });
    }

    private void turnOff(ToggleButton toggle)
    {
        for(ToggleButton item : toggleButtons)
            if(!item.equals(toggle))
                item.setChecked(false);
    }

}
