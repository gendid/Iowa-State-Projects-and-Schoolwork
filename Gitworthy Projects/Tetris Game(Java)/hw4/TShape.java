package hw4;

import java.awt.Color;

import api.Block;
import api.Cell;
import api.Position;

/**
 * Subclass for the T-shaped Shape
 * 
 * @author Gendid
 *
 */
public class TShape extends AbstractShape
{
	/**
	 * Creates a T-shaped shape
	 * 
	 * @param position
	 *            Origin position of the shape
	 * @param magic
	 *            Determines if the shape is magic
	 */
	public TShape(Position position, boolean magic) {
		super(position, magic, false);
		Position position1 = new Position(position.row() - 1, position.col());
		Position position2 = new Position(position1.row() + 1, position1.col() - 1);
		Position position3 = new Position(position2.row(), position2.col() + 2);
		super.setCell(new Cell(new Block(Color.MAGENTA, magic), position1),
				new Cell(new Block(Color.MAGENTA, false), position2),
				new Cell(new Block(Color.MAGENTA, false), position),
				new Cell(new Block(Color.MAGENTA, false), position3));
	}

}
