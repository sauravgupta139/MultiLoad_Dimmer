package com.project.embedded.multiloaddimmer;

import java.io.IOException;
import java.io.OutputStream;
import java.util.UUID;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Spinner;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

public class MainActivity extends Activity {
    private static final String TAG = "LEDOnOff";
    int count=0,count1=0,send,send1;
    private ToggleButton btnFan1,btnFan2,Settable_button,Settable_button1;
    private Button button,plus_but1,plus_but2,minus_but1,minus_but2;
    private Spinner spin,spin1,spin_final,spin_final1;
    private TextView Display_speed1,Display_speed2;
    private static final int REQUEST_ENABLE_BT = 1;
    boolean SwitchOnFan1=false;
    boolean SwitchOnFan2=false;
    boolean Timer = false,Timer1=false;
    boolean isConnected =false;
    private BluetoothAdapter btAdapter = null;
    private BluetoothSocket btSocket = null;
    private OutputStream outStream = null;

    // Well known SPP UUID
    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    // Server's MAC address
    private static String address = "98:D3:31:80:67:A7"; //use capital letters only

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Log.d(TAG, "In onCreate()");

        setContentView(R.layout.content_main);

        //Attach variables to the buttons in the xml file
        btnFan1 = (ToggleButton) findViewById(R.id.Fan1);
        btnFan2 = (ToggleButton) findViewById(R.id.Fan2);
        //button = (Button) findViewById(R.id.button);
        Settable_button =(ToggleButton) findViewById(R.id.settable_Button);
        Settable_button1 =(ToggleButton) findViewById(R.id.settable_Button1);
        plus_but1 = (Button) findViewById(R.id.plus_button1);
        plus_but2 = (Button) findViewById(R.id.plus_button2);
        minus_but1 = (Button) findViewById(R.id.minus_button1);
        minus_but2 = (Button) findViewById(R.id.minus_button2);
        Display_speed1 =(TextView) findViewById(R.id.Display);
        Display_speed2 =(TextView) findViewById(R.id.Display1);
        spin =(Spinner) findViewById(R.id.spinner);
        spin1 =(Spinner) findViewById(R.id.spinner2);
        spin_final =(Spinner) findViewById(R.id.spinner3);
        spin_final1 =(Spinner) findViewById(R.id.spinner4);

        btAdapter = BluetoothAdapter.getDefaultAdapter();

        String[] items = new String[]{"Choose Timer", "1 Hour", "2 Hour", "3 Hour"};
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_dropdown_item, items);
        // Drop down layout style - list view with radio button
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spin.setAdapter(adapter);
        spin1.setAdapter(adapter);

        String[] items1 = new String[]{"Choose Final Value", "0","1", "2", "3","4","5","6","7","8"};
        ArrayAdapter<String> adapter1 = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_dropdown_item, items1);
        // Drop down layout style - list view with radio button
        adapter1.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spin_final.setAdapter(adapter1);
        spin_final1.setAdapter(adapter1);

        checkBTState();
        Connect();
        spin.setOnItemSelectedListener(new OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                if (SwitchOnFan1){
                    switch (position) {
                        case 0:
                            Toast msg = Toast.makeText(getBaseContext(), "Choose Timer", Toast.LENGTH_SHORT);
                            msg.show();
                            break;
                        case 1:
                            sendData("P");
                            Toast.makeText(getBaseContext(), "P", Toast.LENGTH_SHORT).show();
                            break;
                        case 2:
                            sendData("Q");
                            Toast.makeText(getBaseContext(), "Q", Toast.LENGTH_SHORT).show();
                            break;
                        case 3:
                            sendData("R");
                            Toast.makeText(getBaseContext(), "R", Toast.LENGTH_SHORT).show();
                            break;
                    }
                 }
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {
                // TODO Auto-generated method stub
                Toast.makeText(getBaseContext(), "Nothing", Toast.LENGTH_SHORT).show();
            }
        });

        spin1.setOnItemSelectedListener(new OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                if(SwitchOnFan2) {
                    switch (position) {
                        case 0:
                            Toast msg = Toast.makeText(getBaseContext(), "Choose Timer", Toast.LENGTH_SHORT);
                            msg.show();
                            break;
                        case 1:
                            sendData("S");
                            Toast.makeText(getBaseContext(), "S", Toast.LENGTH_SHORT).show();
                            break;
                        case 2:
                            sendData("T");
                            Toast.makeText(getBaseContext(), "T", Toast.LENGTH_SHORT).show();
                            break;
                        case 3:
                            sendData("U");
                            Toast.makeText(getBaseContext(), "U", Toast.LENGTH_SHORT).show();
                            break;
                    }
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {
                // TODO Auto-generated method stub
                Toast.makeText(getBaseContext(), "Nothing", Toast.LENGTH_SHORT).show();
            }
        });

        spin_final.setOnItemSelectedListener(new OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                if(SwitchOnFan1) {
                    switch (position) {
                        case 0:
                            Toast msg = Toast.makeText(getBaseContext(), "Choose Final Value", Toast.LENGTH_SHORT);
                            msg.show();
                            break;
                        case 1:
                            sendData("a");
                            Toast.makeText(getBaseContext(), "a", Toast.LENGTH_SHORT).show();
                            break;
                        case 2:
                            sendData("b");
                            Toast.makeText(getBaseContext(), "b", Toast.LENGTH_SHORT).show();
                            break;
                        case 3:
                            sendData("c");
                            Toast.makeText(getBaseContext(), "c", Toast.LENGTH_SHORT).show();
                            break;
                        case 4:
                            sendData("d");
                            Toast.makeText(getBaseContext(), "d", Toast.LENGTH_SHORT).show();
                            break;
                        case 5:
                            sendData("e");
                            Toast.makeText(getBaseContext(), "e", Toast.LENGTH_SHORT).show();
                            break;
                        case 6:
                            sendData("f");
                            Toast.makeText(getBaseContext(), "f", Toast.LENGTH_SHORT).show();
                            break;
                        case 7:
                            sendData("g");
                            Toast.makeText(getBaseContext(), "g", Toast.LENGTH_SHORT).show();
                            break;
                        case 8:
                            sendData("h");
                            Toast.makeText(getBaseContext(), "h", Toast.LENGTH_SHORT).show();
                            break;
                        case 9:
                            sendData("i");
                            Toast.makeText(getBaseContext(), "i", Toast.LENGTH_SHORT).show();
                            break;
                    }
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {
                // TODO Auto-generated method stub
                Toast.makeText(getBaseContext(), "Nothing", Toast.LENGTH_SHORT).show();
            }
        });

        spin_final1.setOnItemSelectedListener(new OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                if(SwitchOnFan2) {
                    switch (position) {
                        case 0:
                            Toast msg = Toast.makeText(getBaseContext(), "Choose Final Value", Toast.LENGTH_SHORT);
                            msg.show();
                            break;
                        case 1:
                            sendData("j");
                            Toast.makeText(getBaseContext(), "j", Toast.LENGTH_SHORT).show();
                            break;
                        case 2:
                            sendData("k");
                            Toast.makeText(getBaseContext(), "k", Toast.LENGTH_SHORT).show();
                            break;
                        case 3:
                            sendData("l");
                            Toast.makeText(getBaseContext(), "l", Toast.LENGTH_SHORT).show();
                            break;
                        case 4:
                            sendData("m");
                            Toast.makeText(getBaseContext(), "m", Toast.LENGTH_SHORT).show();
                            break;
                        case 5:
                            sendData("n");
                            Toast.makeText(getBaseContext(), "n", Toast.LENGTH_SHORT).show();
                            break;
                        case 6:
                            sendData("o");
                            Toast.makeText(getBaseContext(), "o", Toast.LENGTH_SHORT).show();
                            break;
                        case 7:
                            sendData("p");
                            Toast.makeText(getBaseContext(), "p", Toast.LENGTH_SHORT).show();
                            break;
                        case 8:
                            sendData("q");
                            Toast.makeText(getBaseContext(), "q", Toast.LENGTH_SHORT).show();
                            break;
                        case 9:
                            sendData("r");
                            Toast.makeText(getBaseContext(), "r", Toast.LENGTH_SHORT).show();
                            break;
                    }
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {
                // TODO Auto-generated method stub
                Toast.makeText(getBaseContext(), "Nothing", Toast.LENGTH_SHORT).show();
            }
        });

        btnFan1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton v, boolean isChecked) {
                Connect();
                if (isChecked) {
                    SwitchOnFan1 = true;
                    sendData1(count);
                    sendData("I");
                } else {
                    sendData("0");
                    sendData("J");
                    SwitchOnFan1 = false;
                }
            }
        });

        btnFan2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton v, boolean isChecked) {
                Connect();
                if (isChecked) {
                    SwitchOnFan2 = true;
                    sendData2(count1);
                    sendData("K");
                } else {
                    sendData("9");
                    sendData("L");
                    SwitchOnFan2 = false;
                }
            }
        });

        Settable_button.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton v, boolean isChecked) {
                Connect();
                    if (isChecked) {
                        Timer = true;
                        sendData("W");
                    } else {
                        Timer = false;
                        sendData("X");
                    }
            }
        });

        Settable_button1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton v, boolean isChecked) {
                Connect();
                    if (isChecked) {
                        Timer1 = true;
                        sendData("Y");
                    } else {
                        Timer1 = false;
                        sendData("Z");
                    }
            }
        });

        plus_but1.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                Connect();
                if (SwitchOnFan1) {
                    if (count < 8) {
                        count++;
                    }
                    Display_speed1.setText(String.valueOf(count));
                    sendData1(count);
                }
            }
        });

        plus_but2.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
            Connect();
                if(SwitchOnFan2) {
                    if(count1<8) {
                        count1++;
                    }
                    Display_speed2.setText(String.valueOf(count1));
                    sendData2(count1);
                }
            }
        });

        minus_but1.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
            Connect();
                if(SwitchOnFan1) {
                    if(count>0) {
                        count--;
                    }
                    Display_speed1.setText(String.valueOf(count));
                    sendData1(count);
                }
            }
        });

        minus_but2.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
            Connect();
                if (SwitchOnFan2) {
                    if(count1>0) {
                        count1--;
                    }
                    Display_speed2.setText(String.valueOf(count1));
                    sendData2(count1);
                }
            }
        });

        /*button.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                Connect();
            }
        });*/
    }

    @Override
    public void onStart() {
        super.onStart();
    }
    @Override
    public void onResume() {
        super.onResume();
    }

    @Override
    public void onPause() {
        super.onPause();

        Log.d(TAG, "...In onPause()...");

        if (outStream != null) {
            try {
                outStream.flush();
            } catch (IOException e) {
                Toast.makeText(getBaseContext(), "failed to flush output stream", Toast.LENGTH_SHORT).show();
                //errorExit("Fatal Error", "In onPause() and failed to flush output stream: " + e.getMessage() + ".");
            }
        }

        try     {
            btSocket.close();
        } catch (IOException e2) {
            //errorExit("Fatal Error", "In onPause() and failed to close socket." + e2.getMessage() + ".");
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    private void checkBTState() {
        // Check for Bluetooth support and then check to make sure it is turned on

        // Emulator doesn't support Bluetooth and will return null
        if(btAdapter==null) {
            //errorExit("Fatal Error", "Bluetooth Not supported. Aborting.");
            Toast.makeText(getBaseContext(),"No Bluetooth Adapter found",Toast.LENGTH_SHORT).show();
        }
        else {
            if (btAdapter.isEnabled()) {
                Log.d(TAG, "...Bluetooth is enabled...");
            }
            else {
                btAdapter.enable();
                //Prompt user to turn on Bluetooth
                //Intent enableBtIntent = new Intent(btAdapter.ACTION_REQUEST_ENABLE);
                //startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            }
        }
    }

    private void errorExit(String title, String message){
        Toast msg = Toast.makeText(getBaseContext(), title + " - " + message, Toast.LENGTH_SHORT);
        msg.show();
        finish();
    }

    private void sendData(String message) {
        byte[] msgBuffer = message.getBytes();

        Log.d(TAG, "...Sending data: " + message + "...");

        try {
            outStream.write(msgBuffer);
        } catch (IOException e) {
            String msg = "In onResume() and an exception occurred during write: " + e.getMessage();
            if (address.equals("00:00:00:00:00:00"))
                msg = msg + ".\n\nUpdate your server address from 00:00:00:00:00:00 to the correct address on line 37 in the java code";
            msg = msg +  ".\n\nCheck that the SPP UUID: " + MY_UUID.toString() + " exists on server.\n\n";
            Toast.makeText(getBaseContext(), "Data not sent", Toast.LENGTH_SHORT).show();

            //errorExit("Fatal Error", msg);
        }
    }

    private void sendData1(int count) {
        switch (count) {
            case 0:
                sendData("0");
                break;
            case 1:
                sendData("1");
                break;
            case 2:
                sendData("2");
                break;
            case 3:
                sendData("3");
                break;
            case 4:
                sendData("4");
                break;
            case 5:
                sendData("5");
                break;
            case 6:
                sendData("6");
                break;
            case 7:
                sendData("7");
                break;
            case 8:
                sendData("8");
                break;
        }
    }

    private void sendData2(int count1){
        switch (count1){
                case 0:
                    sendData("9");
                    break;
                case 1:
                    sendData("A");
                    break;
                case 2:
                    sendData("B");
                    break;
                case 3:
                    sendData("C");
                    break;
                case 4:
                    sendData("D");
                    break;
                case 5:
                    sendData("E");
                    break;
                case 6:
                    sendData("F");
                    break;
                case 7:
                    sendData("G");
                    break;
                case 8:
                    sendData("H");
                    break;
            }
        }

    private void Connect(){
        if(isConnected==false) {
            Log.d(TAG, "...In onResume - Attempting client connect...");
            //Toast.makeText(getBaseContext(), "Attempting client connect...", Toast.LENGTH_SHORT).show();

            // Set up a pointer to the remote node using it's address.
            BluetoothDevice device = btAdapter.getRemoteDevice(address);
            //if(btSocket.isConnected())
            //Toast.makeText(getApplicationContext(), "BT Connected", Toast.LENGTH_SHORT).show();

            // Two things are needed to make a connection:
            //   A MAC address, which we got above.
            //   A Service ID or UUID.  In this case we are using the
            //     UUID for SPP.
            try {
                btSocket = device.createRfcommSocketToServiceRecord(MY_UUID);
            } catch (IOException e) {
                Toast.makeText(getBaseContext(), "Connect the device first", Toast.LENGTH_SHORT).show();
                //errorExit("Fatal Error", "In onResume() and socket create failed: " + e.getMessage() + ".");
            }

            // Discovery is resource intensive.  Make sure it isn't going on
            // when you attempt to connect and pass your message.
            btAdapter.cancelDiscovery();

            // Establish the connection.  This will block until it connects.
            Log.d(TAG, "...Connecting to Remote...");
            try {
                btSocket.connect();
                Log.d(TAG, "...Connection established and data link opened...");
                Toast.makeText(getBaseContext(), "Is Connected", Toast.LENGTH_SHORT).show();
                isConnected = true;
            } catch (IOException e) {
                try {
                    btSocket.close();
                } catch (IOException e2) {
                    Toast.makeText(getBaseContext(), "Unable to close socket", Toast.LENGTH_SHORT).show();
                    //errorExit("Fatal Error", "In onResume() and unable to close socket during connection failure" + e2.getMessage() + ".");
                }
            }

            // Create a data stream so we can talk to server.
            Log.d(TAG, "...Creating Socket...");

            try {
                outStream = btSocket.getOutputStream();
            } catch (IOException e) {
                Toast.makeText(getBaseContext(), "output stream creation failed", Toast.LENGTH_SHORT).show();
                //errorExit("Fatal Error", "In onResume() and output stream creation failed:" + e.getMessage() + ".");
            }
        }

    }
}