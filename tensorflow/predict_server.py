import tensorflow as tf
import http.server

session = tf.Session()
input = tf.placeholder(tf.float32, shape=(None))
output = input + 22

class PostHandler(http.server.CGIHTTPRequestHandler):
    def do_POST(self):
        data = self.rfile.read(100)
        if data.isdigit():
            print (b"get " + data)
            result = session.run(output, feed_dict={input: int(data)})
            print(result)
        else:
            self.wfile.write(b"input number")
            return
        self.wfile.write(b"bye")
        return

server = http.server.HTTPServer(("127.0.0.1", 14333), PostHandler)
server.serve_forever()