import java.io.IOException;
import java.net.InetAddress;

public class App {

    public static void main(String[] args) throws IOException {
        InetAddress address = InetAddress.getLocalHost();
        int port = 8000;
        try (Client client = new Client(address, port)) {
            client.call("Hello");
            client.call("World");
            client.call("TCP");
            client.call("rulez");
        }
    }
}
