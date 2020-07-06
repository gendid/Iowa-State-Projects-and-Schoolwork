package hw4;

import java.awt.Color;

import api.Block;
import api.Cell;
import api.Position;
import api.Shape;
import examples.SampleShape;

/**
 * IShape, as in I-yi-yi this guy was trouble. This is the subclass for the
 * I-shaped shape.
 * 
 * @author Gendid Will Springer
 */
public class IShape extends AbstractShape
{
	/**
	 * Creates a new I-shaped shape, as in 3 blocks stacked vertically
	 * 
	 * @param position
	 *            The origin position of the shape
	 * @param magic
	 *            If the shape is magic or not
	 */
	public IShape(Position position, boolean magic) {
		super(position, magic, true);
		Position position1 = new Position(position.row() + 1, position.col());
		Position position2 = new Position(position1.row() + 1, position1.col());
		super.setCell(new Cell(new Block(Color.CYAN, magic), position),
				new Cell(new Block(Color.CYAN, false), position1), new Cell(new Block(Color.CYAN, false), position2));

	}

}
