package hw4;

import java.awt.Color;

import api.Block;
import api.Cell;
import api.Position;

/**
 * Subclass for the square shaped shape
 * 
 * @author Gendid
 *
 */
public class OShape extends AbstractShape
{
	/**
	 * Creates a square shaped shape
	 * 
	 * @param position
	 *            Origin position of the shape
	 * @param magic
	 *            Deterimines if the shape is magic
	 */
	public OShape(Position position, boolean magic) {
		super(position, magic, false);
		Position position1 = new Position(position.row(), position.col() + 1);
		Position position2 = new Position(position1.row() + 1, position1.col() - 1);
		Position position3 = new Position(position2.row(), position2.col() + 1);
		super.setCell(new Cell(new Block(Color.YELLOW, magic), position),
				new Cell(new Block(Color.YELLOW, false), position1),
				new Cell(new Block(Color.YELLOW, false), position2),
				new Cell(new Block(Color.YELLOW, false), position3));

	}

	/**
	 * Transforming this shape does nothing, so this function overrides to do
	 * nothing
	 */
	@Override
	public void transform()
		{

		}

}
