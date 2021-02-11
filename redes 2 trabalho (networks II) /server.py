#!/usr/bin/python3
#|**********************************************************************;
#* Project           : Tweet Mining via UDP stream
#*
#* Program name      : server.py
#*
#* Author            : Arianne de Paula Bortolan GRR20140220
#                    : Dante da Silva Aleo GRR20171593
#*
#* Date created      : 24/05/2019
#*
#* Purpose           : Stream tweets do clients who can do data mining on it
#*
#* Last Edit         : 24/05/2019
#*
#|**********************************************************************;
import threading
import socket
import logging
import sys
import time
#Import the necessary methods for stream
import json
import tweepy
from tweepy.streaming import StreamListener
from tweepy import OAuthHandler
from tweepy import Stream

#Variables that contains the developer credentials to access Twitter API
access_token = "601979779-y4GsY1hhZVApuaTtf3m6WsvXWyCFU734X28UuBEz"
access_token_secret = "fzMj2SoihoKBaLhxR5ZdxTYXNTGiEzve3Aj85mBnKYsFE"
consumer_key = "nQDGE9ml0S5P8wMR0MABLJgYE"
consumer_secret = "4Es0ihVNUx0qvZsLTj2oE5CYoRQcili3YAT0IB3t7Ye6Li9vtn"

#This is a basic listener that just prints received tweets to stdout.
class StdOutListener(StreamListener):

    def on_data(self, data):
        file = open('stream_tweets.txt','a')
        parse = json.loads(data)
        tweets = parse["text"]
        if (tweets != '\n'):
            file.writelines(tweets)
        file.close()
        return True

    def on_error(self, status):
        print (status)

class Server():

    #init the socket class
    def __init__(self,porta,sleeping):
        logging.info('Starting Server')
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind(('', porta))
        self.clients_list = []
        self.sleeping = sleeping

    #function that send the tweets stream to client
    def talkToClient(self, ip, sleeping):
        logging.info("Sending message to client: %s", ip)
        file = open('stream_tweets.txt','r')
        package = 1
        while True:
            data = str(package)+'〄'+str(file.readline())
            self.sock.sendto(bytes(data, "utf-8"), ip)
            time.sleep(sleeping)
            package= package +1

    #our server listen to any clients who wants to conect, start the tweets scrapper and create the threads
    #that will send the tweets
    def listen_clients(self,sleeping):
        listen = StdOutListener()
        auth = OAuthHandler(consumer_key, consumer_secret)
        auth.set_access_token(access_token, access_token_secret)
        stream = Stream(auth, listen)
        #A thread with the stream will be created
        stream.filter(track=['infosec','hacking','cloud','python'], is_async=True)
        while True:
            msg, client = self.sock.recvfrom(1024)
            logging.info('Client %s with adress %s: logged in', msg, client)
            self.clients_list.append(client)
            t = threading.Thread(target=self.talkToClient, args=(client,sleeping))
            t.start()

if __name__ == '__main__':
    # Make sure all log messages show up
    logging.getLogger().setLevel(logging.INFO)
    try:
        porta = int(sys.argv[1])
        sleeping = int(sys.argv[2])
    except:
        print("To run Server: python3 server.py port interval-time-seconds")
        print("Recommended time for interval: 5 seconds")
        sys.exit(1)
    b = Server(porta,sleeping)
    b.listen_clients(sleeping)
