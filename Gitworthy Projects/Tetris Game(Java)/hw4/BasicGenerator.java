/**
 * @author Gendid Will Springer Yup, made this too. Of all the methods in this
 *         assignment, this was the easiest to make. Which probably means I did
 *         it wrong, but we'll see when I get my grade back. Oh, it's also a
 *         Shape generator. More info below.
 */
package hw4;

import java.util.Random;

import api.Generator;
import api.Position;
import api.Shape;
import examples.SampleShape;

/**
 * Generator for Shape objects in MagicTetris. All six shapes are equally
 * likely, and the generated shape is magic with 20% probability. Yup, made this
 * too. Of all the methods in this assignment, this was the easiest to make.
 * Which probably means I did it wrong, but we'll see when I get my grade back.
 * 
 * @author Gendid Will Springer
 */
public class BasicGenerator implements Generator
{
	/**
	 * Returns the next shape for the game, in a predefined position. All shapes are
	 * equally likely to come up Each block has a 20% chance to be a magic shape
	 */
	@Override
	public Shape getNext(int width)
		{
			int mid = width / 2;
			Random rand = new Random();
			int choice = rand.nextInt(6) + 1;
			int isMagic = rand.nextInt(100) + 1;
			boolean magic = false;
			if (isMagic >= 80) {
				magic = true;
			}
			switch (choice)
				{
					case 1:
						return new IShape(new Position(-2, mid), magic);
					case 2:
						return new JShape(new Position(-1, mid), magic);
					case 3:
						return new LShape(new Position(-1, mid + 1), magic);
					case 4:
						return new TShape(new Position(0, mid), magic);
					case 5:
						return new OShape(new Position(-1, mid), magic);
					case 6:
						return new SZShape(new Position(-1, mid), magic);
				}
			return null;
		}
}
