import java.io.*;
import java.net.*;


public class server{
	private static DatagramSocket sock = null;
	private static DatagramPacket packet;
	private static byte[] dataBuffer = new byte[1024];
	private static int port;
	private static String msg;
	private static int windowSize = 4, noOfPackets, count, lastAck;
	// private static int listToSend[windowSize];
	private static InetAddress clientIP;

	static void createSocket() {
		try{
			sock = new DatagramSocket(port);
		} catch(Exception e) {
			System.out.println(e.getMessage());
		}
	}


	static String recvOnePacket() {
		String tmp = "";
		try {
			packet = new DatagramPacket(dataBuffer, dataBuffer.length);
			sock.receive(packet);
			clientIP = packet.getAddress();
			port = packet.getPort();
			tmp = new String(packet.getData());
		} catch(Exception e){
			System.out.println(e.getMessage());
		}
		return tmp;
	} 

	static void sendOnePacket(String tmp) {
		try {
			dataBuffer = tmp.getBytes();
			packet = new DatagramPacket(dataBuffer, dataBuffer.length, clientIP, port);
			sock.send(packet);
		} catch(Exception e){
			System.out.println(e.getMessage());
		}
	}


	static void runServer() {
		try{
			System.out.println("Server Running\n");
			msg = recvOnePacket();
			// System.out.println("Here 1" + msg);
			count = 0;
			// windowSize = 4;
			// listToSend = new int[windowSize];
			noOfPackets = Integer.parseInt(msg.trim());
			// System.out.println("Here 2" + msg);
			

			while(count < noOfPackets) {
				int i=0;
				while(i<windowSize && (count+i) < noOfPackets) {
					String sendString = (count+i)+"*--*Data";
					sendOnePacket(sendString);
					i++;
				}
				//replace this
				count +=4;
			}

			



		} catch (Exception e) {
			// System.out.println(e.getMessage());
			e.printStackTrace();
		}

	}


	public static void main(String args[])throws IOException, UnknownHostException{
			

		port = Integer.parseInt(args[0]);


		createSocket();

		runServer();


	}
}