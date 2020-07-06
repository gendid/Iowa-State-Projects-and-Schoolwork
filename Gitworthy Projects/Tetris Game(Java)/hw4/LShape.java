package hw4;

import java.awt.Color;

import api.Block;
import api.Cell;
import api.Position;

/**
 * Subclass for the L-shaped shape.
 * 
 * @author Gendid Will Springer
 *
 */
public class LShape extends AbstractShape
{
	/**
	 * Creates an L-shaped shape
	 * 
	 * @param position
	 *            Origin of the shape
	 * @param magic
	 *            Determines if the block is magic
	 */
	public LShape(Position position, boolean magic) {
		super(position, magic, false);
		Position position1 = new Position(position.row() + 1, position.col() - 2);
		Position position2 = new Position(position1.row(), position1.col() + 1);
		Position position3 = new Position(position2.row(), position2.col() + 1);
		super.setCell(new Cell(new Block(Color.ORANGE, magic), position),
				new Cell(new Block(Color.ORANGE, false), position1),
				new Cell(new Block(Color.ORANGE, false), position2),
				new Cell(new Block(Color.ORANGE, false), position3));
	}

}
