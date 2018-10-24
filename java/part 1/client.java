import java.io.*;
import java.net.*;


public class client{

	private static DatagramSocket sock = null;
	private static DatagramPacket packet;
	private static InetAddress serverIP;
	private static int serverPort, count, maxPacket, windowSize, int lastAck;
	private static byte[] dataBuffer = new byte[1024];
	private static String msg;



	static void createSocket() {
		try{
			sock = new DatagramSocket();
			// server
		} catch(Exception e) {
			System.out.println(e.getMessage());
		}
	}


	static String recvOnePacket() {
		String tmp = "";
		try {
			packet = new DatagramPacket(dataBuffer, dataBuffer.length );
			sock.receive(packet);
			serverIP = packet.getAddress();
			serverPort = packet.getPort();
			tmp = new String(packet.getData());
			return tmp;
		} catch(IOException e) {
			System.out.println(e.getMessage());
		}
		return tmp;
	}

	static void sendOnePacket(String tmp) {
		try{
			dataBuffer = tmp.getBytes();
			packet = new DatagramPacket(dataBuffer, dataBuffer.length, serverIP, serverPort);
			sock.send(packet);
		} catch(IOException e) {
			System.out.println(e.getMessage());
		}
	}


	static void sendAck(int i) {
		try{
			String tmp = i + "*--*ACK";
			dataBuffer = tmp.getBytes();
			packet = new DatagramPacket(dataBuffer, dataBuffer.length, serverIP, serverPort);
			sock.send(packet);
		} catch (Exception e) {
			System.out.println(e.getMessage());
		}
	}


	static void runClient() {
		try{
			//First message telling how many packets to send
			BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
			msg = in.readLine();
			sendOnePacket(msg);

			//Receiving the packets
			int track = 0;
			lastAck = 0;
			count = 0;
			maxPacket = Integer.parseInt(msg);
			windowSize = 4;

			while(count < maxPacket) {
				int i=0;
				while(i<windowSize && (count+i) < maxPacket) {
					System.out.println(track + "  No of packets recieved");
					System.out.println(recvOnePacket());
					track++;
					i++;
				}
				String ack = recvOnePacket();
				//replace this
				count += 4;

			}

			// msg = recvOnePacket();
			// System.out.println(msg);
		} catch(IOException e) {
			System.out.println(e.getMessage());
		}
	}



	public static void main(String args[])throws IOException, UnknownHostException{
		System.out.println(args[0] + "  " + args[1]);
		
		serverPort = Integer.parseInt(args[1]);
		serverIP = InetAddress.getByName(args[0]);
		createSocket();


		runClient();


	}
}