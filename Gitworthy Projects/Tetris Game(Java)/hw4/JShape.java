package hw4;

import java.awt.Color;

import api.Block;
import api.Cell;
import api.Position;
import api.Shape;

/**
 * This is the subclass for the J-Shaped shape.
 * 
 * @author Gendid Will Springer
 *
 */
public class JShape extends AbstractShape
{
	/**
	 * Creates a J-Shaped shape, kinda like L-Shape but backwords.
	 * 
	 * @param position
	 *            Origin of the point of rotation
	 * @param magic
	 *            Determines if the shape is magic
	 */
	public JShape(Position position, boolean magic) {
		super(position, magic, false);
		Position position0 = new Position(position.row(), position.col() - 1);
		Position position1 = new Position(position0.row() + 1, position0.col());
		Position position2 = new Position(position1.row(), position1.col() + 1);
		Position position3 = new Position(position2.row(), position2.col() + 1);
		super.setCell(new Cell(new Block(Color.BLUE, magic), position0),
				new Cell(new Block(Color.BLUE, false), position1), new Cell(new Block(Color.BLUE, false), position2),
				new Cell(new Block(Color.BLUE, false), position3));
	}

}
