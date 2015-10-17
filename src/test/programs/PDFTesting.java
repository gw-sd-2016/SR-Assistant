package test.programs;

import java.io.IOException;

import org.apache.pdfbox.exceptions.COSVisitorException;
import org.apache.pdfbox.pdmodel.PDDocument;
import org.apache.pdfbox.pdmodel.PDPage;
import org.apache.pdfbox.pdmodel.edit.PDPageContentStream;
import org.apache.pdfbox.pdmodel.font.PDFont;
import org.apache.pdfbox.pdmodel.font.PDType1Font;

public class PDFTesting 
{
	public static void main(String args[]) throws IOException
	{
		System.out.println("Testing program started:");
		PDDocument document = new PDDocument();
		PDPage blankPage = new PDPage();
		document.addPage(blankPage);
		
		//adding content to the page
		PDFont font = PDType1Font.HELVETICA_BOLD;
		PDPageContentStream contentStream = new PDPageContentStream(document, blankPage);
		
		contentStream.beginText();
		contentStream.setFont(font, 12);
		contentStream.moveTextPositionByAmount(100, 700);
		contentStream.drawString("Hello World");
		contentStream.endText();
		
		contentStream.close();
		
		//trying to save
		try {
			document.save("BlankPage.pdf");
		} catch (COSVisitorException | IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			System.out.println("Faield to make document");
		}
		
		//trying to close
		try {
			document.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
}
