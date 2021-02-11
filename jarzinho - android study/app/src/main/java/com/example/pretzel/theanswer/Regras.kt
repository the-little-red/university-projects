package com.example.pretzel.theanswer

import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.TextView

class Regras : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_regras)
    }
    fun Voltar(view: View){
        val sIntent = Intent (this, MainActivity::class.java).apply {

        }
        startActivity(sIntent)

    }
}
