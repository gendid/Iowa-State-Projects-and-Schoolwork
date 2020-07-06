package hw4;

import java.awt.Color;

import api.Block;
import api.Cell;
import api.Position;

/**
 * Creates and transforms the SZShape shape, which flips between being an
 * S-shape and a Z-shape on command
 * 
 * @author Gendid
 *
 */
public class SZShape extends AbstractShape
{
	/**
	 * Creates a new green S-shape
	 * 
	 * @param position
	 *            Origin position of the shape
	 * @param magic
	 *            Determines if the shape is magic
	 */
	public SZShape(Position position, boolean magic) {
		super(position, magic, false);
		Position position1 = new Position(position.row() + 1, position.col());
		Position position2 = new Position(position1.row(), position1.col() + 1);
		Position position3 = new Position(position2.row() + 1, position2.col());
		super.setCell(new Cell(new Block(Color.GREEN, magic), position),
				new Cell(new Block(Color.GREEN, false), position1), new Cell(new Block(Color.GREEN, false), position2),
				new Cell(new Block(Color.GREEN, false), position3));
	}

	/**
	 * Changes the green shape to a red shape, and vice versa Also flips the shape
	 * along the y-axis
	 */
	@Override
	public void transform()
		{
			Cell[] copy = super.getCells();
			if (copy[0].getBlock().getColorHint() == Color.GREEN) {
				Position position = new Position(copy[0].getRow(), copy[0].getCol() + 1);
				Position position1 = new Position(copy[1].getRow(), copy[1].getCol());
				Position position2 = new Position(copy[2].getRow(), copy[2].getCol());
				Position position3 = new Position(copy[3].getRow(), copy[3].getCol() - 1);
				super.setCell(new Cell(new Block(Color.RED, copy[0].getBlock().isMagic()), position),
						new Cell(new Block(Color.RED, copy[1].getBlock().isMagic()), position1),
						new Cell(new Block(Color.RED, copy[2].getBlock().isMagic()), position2),
						new Cell(new Block(Color.RED, copy[3].getBlock().isMagic()), position3));
			} else {
				Position position = new Position(copy[0].getRow(), copy[0].getCol() - 1);
				Position position1 = new Position(copy[1].getRow(), copy[1].getCol());
				Position position2 = new Position(copy[2].getRow(), copy[2].getCol());
				Position position3 = new Position(copy[3].getRow(), copy[3].getCol() + 1);
				super.setCell(new Cell(new Block(Color.GREEN, copy[0].getBlock().isMagic()), position),
						new Cell(new Block(Color.GREEN, copy[1].getBlock().isMagic()), position1),
						new Cell(new Block(Color.GREEN, copy[2].getBlock().isMagic()), position2),
						new Cell(new Block(Color.GREEN, copy[3].getBlock().isMagic()), position3));
			}
		}

}
