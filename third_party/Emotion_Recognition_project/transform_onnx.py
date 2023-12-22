import tf2onnx
import tensorflow as tf

# 加载 TensorFlow 模型
model = tf.keras.models.load_model('model3.h5')

# 转换模型
onnx_model, _ = tf2onnx.convert.from_keras(model, opset=18)

# 保存 ONNX 模型
with open("model3.onnx", "wb") as f:
    f.write(onnx_model.SerializeToString())
