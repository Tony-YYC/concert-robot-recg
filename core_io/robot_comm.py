from flask import Flask
import signal
import robot

app = Flask(__name__)
recg = robot.Recg()

def signal_handler(signal, frame):
    global recg
    del recg
    print("flask server terminated")
    exit()

signal.signal(signal.SIGINT, signal_handler)

@app.route('/robot')
def handle_request():
    beat_result, emo_result = recg.get_result()
    print(f"[Python][robot_comm][INFO]Get http request from remote device and return result Beat result: {beat_result}, Emo result: {emo_result}")
    return {'beat': beat_result, 'emo': emo_result}

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)