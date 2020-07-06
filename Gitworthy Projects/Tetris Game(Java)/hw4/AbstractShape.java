
package hw4;

import java.awt.Color;

import api.Block;
import api.Cell;
import api.Position;
import api.Shape;

/**
 * Abstract superclass for implementations of the Shape interface.It just works!
 * No, really! Seriously!
 * 
 * @author Gendid Will Springer
 */
public abstract class AbstractShape implements Shape
{
	/**
	 * The point of origin position of the current shape
	 */
	private Position	currentPosition;
	/**
	 * An array of blocks representing the shape in its current form
	 */
	private Cell[]		cells;

	/**
	 * Partial constructor to construct a new Shape Sets origin position and cell
	 * array,
	 * 
	 * @param position
	 *            This is the origin position
	 * @param magic
	 *            This determines the magic status of the blocks
	 * @param isIShape
	 *            This determines if the shape is an I(I is special)
	 */
	protected AbstractShape(Position position, boolean magic, boolean isIShape) {

		currentPosition = position;
		if (isIShape) {
			cells = new Cell[3];
		} else {
			cells = new Cell[4];
		}

	}

	/**
	 * Sets up the various shapes
	 * 
	 * @param a
	 *            First block
	 * @param b
	 *            Second block
	 * @param c
	 *            Third block
	 * @param d
	 *            Fourth block
	 */
	protected void setCell(Cell a, Cell b, Cell c, Cell d)
		{
			cells[0] = a;
			cells[1] = b;
			cells[2] = c;
			cells[3] = d;

		}

	/**
	 * Same as above, used to set up IShape
	 * 
	 * @param a
	 *            First block
	 * @param b
	 *            Second block
	 * @param c
	 *            Third block
	 */
	protected void setCell(Cell a, Cell b, Cell c)
		{
			cells[0] = a;
			cells[1] = b;
			cells[2] = c;
		}

	/**
	 * Creates a copy of the current Shape cell array and returns it
	 */
	@Override
	public Cell[] getCells()
		{
			Cell[] copy = new Cell[cells.length];
			for (int i = 0; i < copy.length; i++) {
				copy[i] = new Cell(cells[i]);
			}
			return copy;
		}

	/**
	 * Moves the shape down
	 */
	@Override
	public void shiftDown()
		{
			currentPosition.setRow(currentPosition.row() + 1);
			for (int i = 0; i < cells.length; i++) {
				cells[i].setRow(cells[i].getRow() + 1);
			}

		}

	/**
	 * Moves the shape left
	 */
	@Override
	public void shiftLeft()
		{
			currentPosition.setCol(currentPosition.col() - 1);
			for (int i = 0; i < cells.length; i++) {
				cells[i].setCol(cells[i].getCol() - 1);
			}
		}

	/**
	 * Moves the shape right
	 */
	@Override
	public void shiftRight()
		{
			currentPosition.setCol(currentPosition.col() + 1);
			for (int i = 0; i < cells.length; i++) {
				cells[i].setCol(cells[i].getCol() + 1);
			}
		}

	/**
	 * Moves the magic block within the shape to another block, previous in the
	 * array For example, if the magic is in block 1, it moves it to block 4 Or if
	 * it was in 3, it moves it to 2
	 */
	@Override
	public void cycle()
		{
			Color currentColor = cells[0].getBlock().getColorHint();
			for (int i = 0; i < cells.length; i++) {
				if (cells[i].getBlock().isMagic()) {
					if (i == cells.length - 1) {
						cells[0].setBlock(new Block(currentColor, true));
						cells[cells.length - 1].setBlock(new Block(currentColor, false));
						break;

					} else {
						cells[i].setBlock(new Block(currentColor, false));
						cells[i + 1].setBlock(new Block(currentColor, true));
						break;
					}
				}
			}

		}

	/**
	 * Creates a deep copy of the shape
	 */
	@Override
	public Shape clone()
		{
			try {
				AbstractShape s = (AbstractShape) super.clone();

				// then make it into a deep copy
				s.currentPosition = new Position(currentPosition);
				s.cells = new Cell[cells.length];
				for (int i = 0; i < cells.length; i++) {
					s.cells[i] = new Cell(cells[i]);
				}
				return s;

			} catch (CloneNotSupportedException e) {
				// can't happen
				return null;
			}
		}

	/**
	 * Changes the shape by rotating it counter-clockwise Order of the blocks
	 * remains the same
	 */
	@Override
	public void transform()
		{
			for (int i = 0; i < cells.length; i++) {
				int blockRow = currentPosition.row();
				int blockCol = currentPosition.col();
				int curRow = cells[i].getRow();
				int curCol = cells[i].getCol();
				int tempRow = curRow - blockRow;
				int tempCol = curCol - blockCol;

				cells[i].setRow(-tempCol + blockRow);
				cells[i].setCol(tempRow + blockCol);

			}
		}
}
