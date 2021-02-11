package com.example.pretzel.theanswer

import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.view.View

object Constants {
    @JvmStatic val FIREBASE_PAPEIS: String = "papeis"
    @JvmStatic val FIREBASE_SALA: String = "salas"

}

var INDICE : Int = 1

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
    }

    fun criarJogo(view: View){
        val sIntent = Intent (this, Cjogo::class.java).apply {

        }
        startActivity(sIntent)

    }

    fun entrarSala(view: View){
        val sIntent = Intent (this, SalaEntrar::class.java).apply {

        }
        startActivity(sIntent)
    }

    fun verRegras(view: View){
        val Intent = Intent (this, Regras::class.java).apply{

        }
        startActivity(Intent)
    }

}
