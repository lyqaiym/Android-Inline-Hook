package com.example.hook;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        final Button bt_testStr = findViewById(R.id.bt_test_target_hook);
        bt_testStr.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                NativeTry.testTargetHook();
            }
        });
        final Button bt_test_libc_hook = findViewById(R.id.bt_test_libc_hook);
        bt_test_libc_hook.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                NativeTry.testLibcHook();
            }
        });
    }

}
