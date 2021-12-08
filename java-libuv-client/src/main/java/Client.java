import java.io.*;
import java.net.InetAddress;
import java.net.Socket;

public class Client implements Closeable {

    private final Socket socket;

    private final PrintWriter writer;

    private final BufferedReader reader;

    public Client(InetAddress address, int port) throws IOException {
        this.socket = new Socket(address, port);
        if (!socket.isConnected()) {
            throw new IOException(String.format("Konnte keine Verbindung zum Server %s:%d herstellen", address, port));
        }
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
