package com.amirparsa.bluetoothtester;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Looper;
import android.os.SystemClock;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.material.slider.RangeSlider;
import com.google.android.material.slider.Slider;

import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.List;
import java.util.UUID;
import java.util.concurrent.Callable;
import java.util.zip.Inflater;

public class Control extends AppCompatActivity {

    ImageView volDownBTN,volUpBTN, playPauseBTN, nextBTN, prevBTN;
    RangeSlider rSlider,gSlider,bSlider;
    TextView colorTV;
    Button submitBTN, turnOnBTN, turnOffBTN, randomBTN, customBTN;
    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    boolean playing = true;
    String message;
    InputStream in;
    transient String response;

    // This UUID is unique and fix id for this device
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_control);

        volDownBTN = findViewById(R.id.volumedownBTN);
        volUpBTN = findViewById(R.id.volumeupBTN);
        playPauseBTN = findViewById(R.id.playpauseBTN);
        nextBTN = findViewById(R.id.nextBTN);
        prevBTN = findViewById(R.id.previousBTN);

        turnOnBTN = findViewById(R.id.turnOnBTN);
        turnOffBTN = findViewById(R.id.turnOffBTN);
        randomBTN = findViewById(R.id.randomBTN);
        customBTN = findViewById(R.id.customBTN);

        rSlider = findViewById(R.id.rSlider);
        gSlider = findViewById(R.id.gSlider);
        bSlider = findViewById(R.id.bSlider);

        colorTV = findViewById(R.id.colorTV);
        submitBTN = findViewById(R.id.submitBTN);

        // receive the address of the bluetooth device
        Intent intent = getIntent();
        String address = intent.getStringExtra("addr");

        try {
            if (btSocket == null) {
                myBluetooth = BluetoothAdapter.getDefaultAdapter();

                // This will connect the device with address as passed
                BluetoothDevice hc = myBluetooth.getRemoteDevice(address);
                btSocket = hc.createInsecureRfcommSocketToServiceRecord(myUUID);
                BluetoothAdapter.getDefaultAdapter().cancelDiscovery();

                btSocket.connect();
                in = btSocket.getInputStream();
            }
        } catch (IOException e) {
            e.printStackTrace();
            showToast("Connection Failed!");
            Intent i = new Intent(this, MainActivity.class);
            startActivity(i);
        }

        // Get playing status
        sendRequestUntilResponse("gp");
        playing = response.equals("1");
        if (playing){
            playPauseBTN.setImageResource(R.drawable.pause_image);
        }
        else{
            playPauseBTN.setImageResource(R.drawable.play_image);
        }

        // Get colors status
        sendRequestUntilResponse("gc");
        rSlider.setValues(Arrays.asList(Float.parseFloat(response.substring(0, 3))));
        gSlider.setValues(Arrays.asList(Float.parseFloat(response.substring(3, 6))));
        bSlider.setValues(Arrays.asList(Float.parseFloat(response.substring(6, 9))));
        updateColor();

        playPauseBTN.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                if (btSocket != null) {
                    if (playing) {
                        sendRequest("ps");
                    } else {
                        sendRequest("st");
                    }

                    waitForResponse();
                    if (response.equals("1")) {
                        if (playing) {
                            playPauseBTN.setImageResource(R.drawable.play_image);
                            message = "paused!";
                        } else {
                            playPauseBTN.setImageResource(R.drawable.pause_image);
                            message = "playing...";
                        }
                        showToast(message);
                        playing = !playing;
                    }
                }
            }
        });

        nextBTN.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                sendRequest("ne");
                waitForResponseAndShowText("1","Next...", "Sth went wrong!");
            }
        });

        prevBTN.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                sendRequest("pr");
                waitForResponseAndShowText("1","Previous...", "Sth went wrong!");
            }
        });

        volUpBTN.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                sendRequest("vu");
                waitForResponseAndShowText("1","Done", "Sth went wrong!");
            }
        });

        volDownBTN.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                sendRequest("vd");
                waitForResponseAndShowText("1","Done", "Sth went wrong!");
            }
        });

        submitBTN.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                int redValue = rSlider.getValues().get(0).intValue();
                int greenValue = gSlider.getValues().get(0).intValue();
                int blueValue = bSlider.getValues().get(0).intValue();
                sendRequest(String.format("c%03d%03d%03d", redValue, greenValue, blueValue));
            }
        });

        turnOnBTN.setOnClickListener(new View.OnClickListener(){

            @Override
            public void onClick(View view) {
                sendRequest("pn");
                waitForResponseAndShowText("1","Done", "Sth went wrong!");
            }
        });

        turnOffBTN.setOnClickListener(new View.OnClickListener(){

            @Override
            public void onClick(View view) {
                sendRequest("pf");
                waitForResponseAndShowText("1","Done", "Sth went wrong!");
            }
        });

        randomBTN.setOnClickListener(new View.OnClickListener(){

            @Override
            public void onClick(View view) {
                sendRequest("rn");
                waitForResponseAndShowText("1","Done", "Sth went wrong!");
            }
        });

        customBTN.setOnClickListener(new View.OnClickListener(){

            @Override
            public void onClick(View view) {
                sendRequest("rf");
                waitForResponseAndShowText("1","Done", "Sth went wrong!");
            }
        });

        rSlider.addOnChangeListener(listener());
        gSlider.addOnChangeListener(listener());
        bSlider.addOnChangeListener(listener());
    }

    private RangeSlider.OnChangeListener listener(){
        return new RangeSlider.OnChangeListener() {
            @Override
            public void onValueChange(@NonNull RangeSlider slider, float value, boolean fromUser) {
                updateColor();
            }
        };
    }

    private void sendRequestUntilResponse(String command){
        response = null;
        while (response == null) {
            sendRequest(command);
            waitForResponse();
        }
    }

    private void sendRequest(String message){
        try {
            btSocket.getOutputStream().write(message.getBytes(StandardCharsets.UTF_8));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    private void waitForResponse(){
        Thread responseThread = new ResponseThread();
        try {
            responseThread.start();
            responseThread.join(3000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        if (responseThread.isAlive()){
            responseThread.interrupt();
        }
    }

    private void waitForResponseAndShowText(String expectedValue, String success, String failed){
        waitForResponse();
        if (response.equals(expectedValue)){
            showToast(success);
        }
        else{
            showToast(failed);
        }
    }

    private void updateColor(){
        int redValue = rSlider.getValues().get(0).intValue();
        int greenValue = gSlider.getValues().get(0).intValue();
        int blueValue = bSlider.getValues().get(0).intValue();
        colorTV.setText(String.format("#%02x%02x%02x",redValue,greenValue,blueValue));
        colorTV.setBackgroundColor(Color.parseColor(String.format("#%02x%02x%02x",redValue,greenValue,blueValue)));
    }

    private class ResponseThread extends Thread{

        @Override
        public void run() {
            Looper.prepare();
            byte[] buffer = new byte[1024];  // buffer store for the stream
            int bytes = 0; // bytes returned from read()
            // Keep listening to the InputStream until an exception occurs
            while (true) {
                try {
                    /*
                    Read from the InputStream from Arduino until termination character is reached.
                    Then send the whole String message to GUI Handler.
                     */
                    buffer[bytes] = (byte) in.read();
                    if (buffer[bytes] == '.'){
                        response = new String(buffer,0,bytes);
                        break;
                    } else {
                        bytes++;
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                    break;
                }
            }
        }
    }

    // Toast Message Function
    private void showToast(String msg){
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }
}