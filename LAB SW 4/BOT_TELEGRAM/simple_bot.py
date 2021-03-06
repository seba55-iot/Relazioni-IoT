import telepot
import requests
import json
from telepot.loop import MessageLoop
from telepot.namedtuple import InlineKeyboardMarkup, InlineKeyboardButton


def on_chat_message(msg):
    content_type, chat_type, chat_id = telepot.glance(msg)

    keyboard = InlineKeyboardMarkup(inline_keyboard=[
                   [InlineKeyboardButton(text='Temperatura', callback_data='temp')],
                   [InlineKeyboardButton(text='Led', callback_data='led')],
                   [InlineKeyboardButton(text='Movement', callback_data='move')],
                   [InlineKeyboardButton(text='Air Conditioning ON', callback_data='dcmotor1')],
                   [InlineKeyboardButton(text='Air Conditioning OFF', callback_data='dcmotor2')],
               ])

    bot.sendMessage(chat_id, 'Usa il menu per mostrare/settare i valori della tua SmartHome', reply_markup=keyboard)

    content_type, chat_type, chat_id = telepot.glance(msg)


def on_callback_query(msg):
   
    query_id, from_id, query_data = telepot.glance(msg, flavor='callback_query')
    print('Callback Query:', query_id, from_id, query_data)
    if(query_data == 'temp'):
        response = requests.get("http://seba.local/arduino/temperature")
        data_dict = response.json()
        bot.sendMessage(from_id, text='La Temperatura è di: ' + str(data_dict['e'][0]['v']) + '°C. \n Se vuoi azionare la ventola clicca su "Air conditioning ON"')
    
    if(query_data == 'led'):
        
        response = requests.get(f"http://seba.local/arduino/led")
        data_dict = response.json()
        bot.sendMessage(from_id, text='Il led è: ' + str(data_dict['e'][0]['v']))
        
    if(query_data == 'move'):
        response = requests.get(f"http://seba.local/arduino/move")
        data_dict = response.json()
        bot.sendMessage(from_id, text='Il numero di movimenti rilevati negli ultimi 60 secondi è: ' + str(data_dict['e'][0]['v']))

      
    if(query_data == 'dcmotor1'):
        response = requests.get(f"http://seba.local/arduino/dcmotor1")
        data_dict = response.json()
        bot.sendMessage(from_id, text='La velocità della ventola, in base alla temperatura attuale è di: ' + str(data_dict['e'][0]['v']/51))

    if(query_data == 'dcmotor2'):
        response = requests.get(f"http://seba.local/arduino/dcmotor2")
        data_dict = response.json()
        bot.sendMessage(from_id, text='La velocità della ventola, in base alla temperatura attuale è di: ' + str(data_dict['e'][0]['v']/51))

      

TOKEN = '1042260336:AAFvtdoahjCyFOI76GzNbRJC1A6c_umuIhQ'

bot = telepot.Bot(TOKEN)
MessageLoop(bot, {'chat': on_chat_message,
                  'callback_query': on_callback_query}).run_as_thread()
print('Listening ...')

import time
while 1:
    time.sleep(10)
