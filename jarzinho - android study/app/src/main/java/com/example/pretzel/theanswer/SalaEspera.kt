package com.example.pretzel.theanswer

import android.os.Bundle
import android.app.Activity
import android.content.Intent
import android.view.View


import kotlinx.android.synthetic.main.activity_sala_espera.*

class SalaEspera : Activity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_sala_espera)
    }

    fun SairSala(view: View){
        val sIntent = Intent (this, MainActivity::class.java).apply {

        }
        startActivity(sIntent)

    }

    fun ComeçarJogo(view: View){
        val sIntent = Intent (this, MainActivity::class.java).apply {

        }
        startActivity(sIntent);

    }

}
