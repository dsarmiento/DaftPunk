package com.dsarmiento.daftcontroller;

import android.app.Activity;
import android.app.Dialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.AsyncTask;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import java.io.IOException;
import java.util.LinkedList;
import java.util.Set;
import java.util.UUID;


public class ControllerMain extends AppCompatActivity {
    private LinkedList<ToggleButton> toggleButtons;
    private static int REQUEST_ENABLE_BT = 1;
    private BluetoothAdapter mBluetoothAdapter;
    private BluetoothDevice mBluetoothDevice;
    private BluetoothSocket mBluetoothSocket;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private boolean isBtConnected = false;
    private SeekBar delayBar;

    private AlertDialog ad;

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

        final ToggleButton roboCop          = (ToggleButton) findViewById(R.id.roboCop);
        final ToggleButton heartBeat        = (ToggleButton) findViewById(R.id.heartBeat);
        final ToggleButton heartBlink       = (ToggleButton) findViewById(R.id.heartBlink);
        final ToggleButton blinkingEyes     = (ToggleButton) findViewById(R.id.blinkingEyes);
        final ToggleButton whiteNoise       = (ToggleButton) findViewById(R.id.whiteNoise);
        final ToggleButton marqueeText      = (ToggleButton) findViewById(R.id.marqueeText);
        final ToggleButton staticText       = (ToggleButton) findViewById(R.id.staticText);
        final ToggleButton pacMan           = (ToggleButton) findViewById(R.id.pacMan);
        final ToggleButton allOn            = (ToggleButton) findViewById(R.id.allOn);
        final ToggleButton allOff           = (ToggleButton) findViewById(R.id.allOff);
        final ToggleButton cyclops          = (ToggleButton) findViewById(R.id.cyclops);
        final ToggleButton daftPunk         = (ToggleButton) findViewById(R.id.daftPunk);

        final Button centerButton = (Button) findViewById(R.id.center);

        final EditText sendMsg = (EditText) findViewById(R.id.sendMsg);

        delayBar = (SeekBar) findViewById(R.id.delayBar);
        centerButton.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v){
                delayBar.setProgress(50);
            }
        });

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
        toggleButtons.add(cyclops);
        toggleButtons.add(daftPunk);

        roboCop.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    sendCmd(4);
                    turnOff(roboCop);
                }
            }
        });

        heartBeat.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    sendCmd(2);
                    turnOff(heartBeat);
                }
            }
        });

        heartBlink.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    sendCmd(3);
                    turnOff(heartBlink);
                }
            }
        });

        blinkingEyes.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    sendCmd(6);
                    turnOff(blinkingEyes);
                }
            }
        });

        whiteNoise.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    sendCmd(5);
                    turnOff(whiteNoise);
                }
            }
        });

        marqueeText.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    sendCmd(1);
                    sendMessage(sendMsg.getEditableText().toString());
                    turnOff(marqueeText);
                }
            }
        });

        staticText.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    turnOff(staticText);
                    if(sendMsg.getEditableText().toString().length() <= 6)
                        sendMessage(sendMsg.getEditableText().toString());
                    else
                        msg("Too long", Toast.LENGTH_SHORT);
                }
            }
        });

        pacMan.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    sendCmd(10);
                    turnOff(pacMan);
                }
            }
        });

        allOn.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    sendCmd(8);
                    turnOff(allOn);
                }
            }
        });

        allOff.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    sendCmd(9);
                    turnOff(allOff);
                }
            }
        });

        cyclops.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    sendCmd(11);
                    turnOff(cyclops);
                }
            }
        });

        daftPunk.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    sendCmd(12);
                    turnOff(daftPunk);
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
            lv.setOnItemClickListener(myListClickListener);
            ad = alertDialog.show();
        }
    }

    private AdapterView.OnItemClickListener myListClickListener = new AdapterView.OnItemClickListener()
    {
        public void onItemClick (AdapterView<?> av, View v, int arg2, long arg3)
        {
            ad.dismiss();
            // Get the device MAC address, the last 17 chars in the View
            String info = ((TextView) v).getText().toString();
            String address = info.substring(info.length() - 17);

            mBluetoothDevice = mBluetoothAdapter.getRemoteDevice(address);
            new ConnectBT().execute();


        }
    };

    private void sendCmd(int x) {
        if(mBluetoothSocket != null) {
            try {
                //System.out.println("In sendCmd " + Integer.toString(x));
                mBluetoothSocket.getOutputStream().write(Integer.toString(x).getBytes());
                mBluetoothSocket.getOutputStream().write(",".getBytes());
                mBluetoothSocket.getOutputStream().write(Integer.toString(delayBar.getProgress()).getBytes());
            } catch (IOException e) {}
        }
        else
            msg(Integer.toString(delayBar.getProgress()), 0);
    }

    private void sendMessage(String msg) {
        msg = msg.toUpperCase();
        if(mBluetoothSocket != null) {
            try {
                mBluetoothSocket.getOutputStream().write(msg.getBytes());
            } catch (IOException e) {}
        }
    }

    private void msg(String msgToast, int duration){
        Toast.makeText(getApplicationContext(), msgToast, duration).show();
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


    private class ConnectBT extends AsyncTask<Void, Void, Void> {
        private boolean ConnectSuccess = true; //if it's here, it's almost connected

        @Override
        protected void onPreExecute()
        {

        }

        @Override
        protected Void doInBackground(Void... devices) //while the progress dialog is shown, the connection is done in background
        {
            try
            {
                if (mBluetoothSocket == null || !isBtConnected)
                {
                    mBluetoothSocket = mBluetoothDevice.createInsecureRfcommSocketToServiceRecord(myUUID);//create a RFCOMM (SPP) connection
                    mBluetoothSocket.connect();//start connection
                }
            }
            catch (IOException e)
            {
                ConnectSuccess = false;//if the try failed, you can check the exception here
            }
            return null;
        }
        @Override
        protected void onPostExecute(Void result) //after the doInBackground, it checks if everything went fine
        {
            super.onPostExecute(result);

            if (!ConnectSuccess)
            {
                finish();
                msg("Failed to connect!", Toast.LENGTH_LONG);
            }
            else
            {
                isBtConnected = true;
                msg("Connected", Toast.LENGTH_SHORT);
            }
        }
    }

}
