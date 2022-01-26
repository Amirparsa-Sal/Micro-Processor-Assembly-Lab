package com.amirparsa.bluetoothtester;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

public class MainActivity extends AppCompatActivity {

    private static final int REQUEST_ENABLE_BT = 0;

    ListView listView;
    BluetoothAdapter adapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        adapter = BluetoothAdapter.getDefaultAdapter();
        listView = (ListView) findViewById(R.id.deviceList);

        if (!adapter.isEnabled()){
            showToast("Turning On Bluetooth...");
            Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(intent, REQUEST_ENABLE_BT);
        }

        Set<BluetoothDevice> pairedDevices = adapter.getBondedDevices();
        ArrayList<String> pairedDevicesList = new ArrayList<>();
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice dev : pairedDevices) {
                pairedDevicesList.add(dev.getName() + "\n" + dev.getAddress());
            }
        }
        else{
            showToast("No Paired Device Found!");
        }

        final ArrayAdapter<String> adapter = new ArrayAdapter(this, android.R.layout.simple_list_item_1, pairedDevicesList);
        listView.setAdapter(adapter);

        listView.setOnItemClickListener(myListListener);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode){
            case REQUEST_ENABLE_BT:
                if (requestCode == RESULT_OK){
                    showToast("Bluetooth turned on!");
                }
                else{
                    showToast("Oops sth went wrong!");
                }
                break;
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    // Toast Message Function
    private void showToast(String msg){
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }

    private AdapterView.OnItemClickListener myListListener = new AdapterView.OnItemClickListener() {

        @Override
        public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
            String value = (String)adapterView.getItemAtPosition(i);
            String name = value.split("\n")[0];
            String address = value.split("\n")[1];

            Intent intent = new Intent(MainActivity.this, Control.class);
            intent.putExtra("addr", address);
            intent.putExtra("name", name);
            startActivity(intent);
        }
    };
}