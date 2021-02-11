from flask import Flask, request, abort

from linebot import (
    LineBotApi, WebhookHandler
)
from linebot.exceptions import (
    InvalidSignatureError
)
from linebot.models import (
    MessageEvent, TextMessage, TextSendMessage,
)
import os, json 
import time
import numpy as np

app = Flask(__name__)

#環境変数設定
ACCESS_TOKEN = os.environ["YOUR_CHANNEL_ACCESS_TOKEN"]
SECRET = os.environ["YOUR_CHANNEL_ACCESS_TOKEN"]

line_bot_api = LineBotApi(ACCESS_TOKEN) 
handler = WebhookHandler(SECRET)


@app.route("/callback", methods=['POST'])
def callback():
	
	#ヘッダー
    signature = request.headers['X-Line-Signature']
	
	#リクエストボディ
    body = request.get_data(as_text=True)
    app.logger.info("Request body: " + body)
 
    try: 
        handler.handle(body, signature)
    except InvalidSignatureError:
        abort(400)

    return 'OK'
    
    
global a
a=0


@app.route("/bath", methods=["GET"])
def handle_get_request():
    global a
    
    b = a
    a = 0
    print("accept_"+str(b))
    #lineID
    line_bot_api.push_message("881784f423ca088b1066d27c96f79f8c",TextSendMessage(text="accept_"+str(b)))
    
    return "%%%"+str(b)+"%%%"


bathIn = "お風呂入れておきます！"
bathBoil = ["お風呂沸かしときます！","追い焚きします！"]
bathInWords = ["お風呂入れて","おふろいれて","ohuroirete","おふろ入れて","お風呂いれて","お風呂入れといて","お風呂いれといて"]
bathBoilWords = ["おいだき","追いだき","追い焚き","oidaki","おい焚き"]

falseWords["よくわからん","どういうこと？","うん？","はい？","なんて？"]

@handler.add(MessageEvent, message=TextMessage)
def handle_message(event):
    text = event.message.text
    global a
    
    
    if text in bathInWords:
        line_bot_api.reply_message(event.reply_token,TextSendMessage(text=bathIn))
        a = 1
        

    elif text in bathBoilWords:
        line_bot_api.reply_message(event.reply_token,TextSendMessage(text=bathBoil[np.random.randint(1)]))
        a = 2

    else:
        line_bot_api.reply_message(event.reply_token,TextSendMessage(text=falseWords[np.random.randint(4)]))


if __name__ == "__main__":
    port = int(os.environ.get('PORT', 8000))
    app.run(host ='0.0.0.0',port = port)
	
@app.route('/')
def index():
    return 'Hello world'
