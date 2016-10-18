package com.dsarmiento.daftcontroller;

import android.app.Activity;
import android.app.Dialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.CompoundButton;
import android.widget.ListView;
import android.widget.ToggleButton;

import java.util.LinkedList;
import java.util.Set;


public class ControllerMain extends AppCompatActivity {
    private LinkedList<ToggleButton> toggleButtons;
    private static int REQUEST_ENABLE_BT = 1;
    private BluetoothAdapter mBluetoothAdapter;

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.actionbar_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch(item.getItemId()) {
            case R.id.action_connect:
                if(mBluetoothAdapter != null)
                    connectBluetooth();
                else
                    testDialog();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_controller_main);

        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(mBluetoothAdapter != null) {
            if (!mBluetoothAdapter.isEnabled()) {
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            }
        }

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

    private void turnOff(ToggleButton toggle) {
        for(ToggleButton item : toggleButtons)
            if(!item.equals(toggle))
                item.setChecked(false);
    }

    private void connectBluetooth() {
        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
        if(pairedDevices.size() > 0) {
            AlertDialog.Builder alertDialog = new AlertDialog.Builder(ControllerMain.this);
            LayoutInflater inflater = getLayoutInflater();
            View convertView = (View) inflater.inflate(R.layout.bt_list, null);
            alertDialog.setView(convertView);
            alertDialog.setTitle("Bluetooth Devices");

            ListView lv = (ListView) convertView.findViewById(R.id.bt_listView);
            ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);

            for(BluetoothDevice device : pairedDevices) {
                adapter.add(device.getName() + "\n" + device.getAddress());
            }

            lv.setAdapter(adapter);
            alertDialog.show();
        }
    }

    private void testDialog() {
        AlertDialog.Builder alertDialog = new AlertDialog.Builder(ControllerMain.this);
        LayoutInflater inflater = getLayoutInflater();
        View convertView = (View) inflater.inflate(R.layout.bt_list, null);
        alertDialog.setView(convertView);
        alertDialog.setTitle("Test");

        ListView lv = (ListView) convertView.findViewById(R.id.bt_listView);
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);

        for(int i = 0; i < 5; i++) {
            char name = (char)('A' + i);
            char addr = (char)('0' + i);
            adapter.add(name + "\n" + addr);
        }

        lv.setAdapter(adapter);
        alertDialog.show();
    }

}
