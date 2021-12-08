import java.io.*;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.time.Duration;

public class Client implements Closeable {

    private final Socket socket;

    private final PrintWriter writer;

    private final BufferedReader reader;

    public Client(InetAddress address, int port, Duration timeout) throws IOException {
        this.socket = new Socket();
        this.socket.connect(new InetSocketAddress(address, port), Math.toIntExact(timeout.toMillis()));
        socket.setKeepAlive(true);
        this.writer = new PrintWriter(socket.getOutputStream(), true);
        this.reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
    }

    public void call(String message) throws IOException {
        this.writer.println(message);
        System.out.println(reader.readLine());
    }

    @Override
    public void close() throws IOException {
        socket.close();
    }
}
