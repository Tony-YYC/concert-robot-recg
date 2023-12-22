from flask import Flask
import robot

app = Flask(__name__)
recg = robot.Recg()
@app.route('/robot')
def handle_request():
    beat_result, emo_result = recg.get_result()
    print(f"[Python][robot_comm][INFO]Get http request from remote device and return result Beat result: {beat_result}, Emo result: {emo_result}")
    return {'beat': beat_result, 'emo': emo_result}

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)