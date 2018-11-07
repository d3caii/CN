import java.io.*;

public class LinkState {

	private static int[][] nodes = new int[51][51];
	private static int[] nodesTracker = new int[51];
	private static int noOfNodes;

	private static String input;

    public static void main(String args[])throws IOException {
    
        BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
        input = in.readLine();
	
		//Initialize nodes array
		for(int i=0; i<51; i++) 
			for(int j=0; j<51; j++)
				nodes[i][j] = -1;


		//Parsing the String input
	    String[] lines = input.split(";");
		int linesNo = lines.length;
		for(int i=0; i<linesNo; i++) {
			lines[i] = lines[i].trim();
			String[] link = lines[i].split(", ");
			int val = Integer.parseInt(link[1]);
			String[] tmpNodes = link[0].split(":");
			int node1 = Integer.parseInt(tmpNodes[0]);
			int node2 = Integer.parseInt(tmpNodes[1]);
			nodes[node1][node2] = val;
			nodes[node2][node1] = val;
			nodesTracker[node1] = 1;
			nodesTracker[node2] = 1;
		}

		noOfNodes = findNoOfNodes();
		System.out.println(noOfNodes);
		printNodes();

	}

	//To find the number of nodes
	public static int findNoOfNodes() {
		int len = 0;
		for (int i=0; i<51 ;i++ ) {
			len += nodesTracker[i];
		}
		return len;
	}

	public static void printNodes() {
		for (int i=1; i<=noOfNodes; i++) {
			for(int j=1; j<=noOfNodes; j++) {
				System.out.print(nodes[i][j] + " ");
			}
			System.out.println();
		}
	}

}

